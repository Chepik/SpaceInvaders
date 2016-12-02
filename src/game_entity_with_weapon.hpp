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

  GameEntityWithWeapon(Point2D const & position,
                       std::string const & name,
                       uint const & rate,
                       uint const & health)
    : GameEntity(position,name),
      m_rate(rate),
      m_health(health)
  {}

  ~GameEntityWithWeapon() override;

  uint GetRate() const;
  uint GetHealth() const;
  BulletManager const & GetBulletManager() const;
  void SetRate(uint const & rate);
  void SetHealth(uint const & health);
  void SetBulletManager(BulletManager const & bulletManager);
  
protected:
  uint m_rate = 0;
  uint m_health = 0;
  BulletManager m_bulletManager;
};
