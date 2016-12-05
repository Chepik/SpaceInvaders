#pragma once

#include "game_entity.hpp"


class Obstacle: public GameEntity
{
public:
  Obstacle()
    : GameEntity("Obstacle")
  {}

  Obstacle(uint const & health,
           QVector2D const & position)
    :GameEntity(position, "Obstacle"),
      m_health(health)
  {}

  Obstacle(uint const & health,
           QVector2D const & position,
           std::shared_ptr<QImage> image,
           Box2D const & size)
    :GameEntity(position, "Obstacle", image, size),
      m_health(health)
  {}

  ~Obstacle() override;

  void Update() override;
  uint GetHealth() const;
  void SetHealth(uint const & health);

private:
  uint m_health = 0;
};

using TObstaclePtr = std::shared_ptr<Obstacle>;

std::ostream & operator << (std::ostream & os,
                            Obstacle const & obj);
