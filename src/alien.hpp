#pragma once

#include "game_entity_with_weapon.hpp"

#include <memory>

class Alien : public GameEntityWithWeapon
{
public:
  Alien()
    : GameEntityWithWeapon("Alien")
  {}

  Alien(int const & speed,
        QVector2D const & position,
        uint const & rate,
        int const & health)
    : GameEntityWithWeapon(position, "Alien", rate, health),
      m_speed(speed)
  {}

  Alien(int const & speed,
        QVector2D const & position,
        uint const & rate,
        int const & health,
        std::shared_ptr<QImage> image,
        TSize const & size,
        uint const & frequency)
    : GameEntityWithWeapon(position, "Alien", rate, health, image, size),
      m_speed(speed),
      m_shotTime(frequency),
      m_frequency(frequency)
  {}

  ~Alien() override;

  void Move() override;
  void Update() override;
  int GetSpeed() const;
  int GetAbsoluteSpeed() const;
  void SetSpeed(int const & rate);
  void IncreaseX(float const & value) override;
  void DecreaseX(float const & value) override;
  bool Shot();
  
private:
  // Change direction.
  void ReverseDirection();

  uint m_speed = 0;
  uint m_shotTime = 0;
  uint m_frequency = 0;

};

using TAlienPtr = std::shared_ptr<Alien>;

std::ostream & operator << (std::ostream & os,
                            Alien const & obj);
