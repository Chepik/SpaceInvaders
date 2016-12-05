#pragma once

#include "game_entity.hpp"
#include "movable_interface.hpp"

#include <QVector2D>
#include <QOpenGLTexture>

class Bullet : public GameEntity, public MovableInterface
{
public:
  Bullet()
    : GameEntity("Bullet")
  {}
  
  Bullet(uint const & damage, QVector2D const & position)
    : GameEntity(position, "Bullet"),
      m_damage(damage)
  {}

  Bullet(QVector2D const & position,
         std::shared_ptr<QImage> image,
         uint const & damage) :
    GameEntity(position, "Bullet", image),
    m_damage(damage)
  {}

  ~Bullet() override;

  void Update() override;
  void Move() override;

  uint GetDamage() const;
  void SetDamage(uint const & damage);
  virtual void IncreaseY(float const &value) override;

private:
  uint m_damage = 0;
};

using TBulletPtr = std::shared_ptr<Bullet>;

std::ostream & operator << (std::ostream & os,
                            Bullet const & obj);
