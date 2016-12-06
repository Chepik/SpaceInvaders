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
        uint const & health)
    : GameEntityWithWeapon(position, "Alien", rate, health),
      m_speed(speed)
  {}

  Alien(int const & speed,
        QVector2D const & position,
        uint const & rate,
        uint const & health,
        std::shared_ptr<QImage> image,
        TSize const & size)
    : GameEntityWithWeapon(position, "Alien", rate, health, image, size),
      m_speed(speed),
      m_shotTime(rate)
  {}

  ~Alien() override;

  void Move() override;
  void Update() override;
  int GetSpeed() const;
  void SetSpeed(int const & rate);
  void IncreaseX(float const & value) override;
  void DecreaseX(float const & value) override;
  bool Shot();
  
private:
  // Change direction.
  void ReverseDirection();

  uint m_speed = 0;
  uint m_shotTime = 0;

};

using TAlienPtr = std::shared_ptr<Alien>;

std::ostream & operator << (std::ostream & os,
                            Alien const & obj);
