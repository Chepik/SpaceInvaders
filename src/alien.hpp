#include "game_entity_with_weapon.hpp"

class Alien : public GameEntityWithWeapon
{
public:
  Alien()
    : GameEntityWithWeapon("Alien")
  {}

  Alien(uint const & speed,
        QVector2D const & position,
        uint const & rate,
        uint const & health)
    : GameEntityWithWeapon(position, "Alien", rate, health),
      m_speed(speed)
  {}

  Alien(uint const & speed,
        QVector2D const & position,
        uint const & rate,
        uint const & health,
        QImage * image)
    : GameEntityWithWeapon(position, "Alien", rate, health, image),
      m_speed(speed)
  {}

  ~Alien() override;

  void Move() override;
  void Update() override;
  uint GetSpeed() const;
  void SetSpeed(uint const & rate);
  
private:
  uint m_speed = 0;
};

std::ostream & operator << (std::ostream & os,
                            Alien const & obj);
