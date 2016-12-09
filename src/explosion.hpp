#pragma once

#include "game_entity.hpp"

#include <QVector2D>
#include <QOpenGLTexture>

class Explosion : public GameEntity
{
public:
  Explosion()
    : GameEntity("Explosion"),
      m_lifetime(120)
  {}
  
  Explosion(QVector2D const & position)
    : GameEntity(position, "Explosion"),
      m_lifetime(120)
  {}

  Explosion(QVector2D const & position,
         std::shared_ptr<QImage> image,
         TSize const & size,
         uint lifetime) :
    GameEntity(position, "Explosion", image, size),
    m_lifetime(lifetime)
  {}

  ~Explosion() override;

  bool ReduceLifeTime();
  void Update() override;

private:
  uint m_lifetime;
};

using TExplosionPtr = std::shared_ptr<Explosion>;

//std::ostream & operator << (std::ostream & os,
//                            Explosion const & obj);
