#pragma once

#include "game_entity.hpp"
#include "movable_interface.hpp"
#include "bullet_manager.hpp"

class GameEntityWithWeapon: public GameEntity, public MovableInterface
{
public:
  GameEntityWithWeapon() = default;

  GameEntityWithWeapon(std::string const & name)
    : GameEntity(name)
  {}

  GameEntityWithWeapon(QVector2D const & position,
                       std::string const & name,
                       uint const & rate,
                       int const & health)
    : GameEntity(position, name),
      m_rate(rate),
      m_health(health)
  {}  

  GameEntityWithWeapon(QVector2D const & position,
                       std::string const & name,
                       uint const & rate,
                       int const & health,
                       std::shared_ptr<QImage> image,
                       TSize const & size)
    : GameEntity(position, name, image, size),
      m_rate(rate),
      m_health(health)
  {}

  ~GameEntityWithWeapon() override;

  uint GetRate() const;
  int GetHealth() const;
  BulletManager const & GetBulletManager() const;
  void SetRate(uint const & rate);
  void SetHealth(int const & health);
  void SetBulletManager(BulletManager const & bulletManager);
  
protected:
  uint m_rate = 0;
  int m_health = 0;
  BulletManager m_bulletManager;
};
