#include "game_entity.hpp"
#include "movable_interface.hpp"

class Bullet: public GameEntity, public MovableInterface
{
public:
  Bullet()
    : GameEntity("Bullet")
  {}
  
  Bullet(uint const & damage, Point2D const & position)
    :GameEntity(position, "Bullet"),
      m_damage(damage)
  {}

  ~Bullet() override;

  void Update() override;
  void Move() override;
  uint GetDamage() const;
  void SetDamage(uint const & damage);

private:
  uint m_damage = 0;
};

using TBulletPtr = std::shared_ptr<Bullet>;

std::ostream & operator << (std::ostream & os,
                            Bullet const & obj);
