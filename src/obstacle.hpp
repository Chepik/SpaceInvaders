#pragma once

#include "game_entity.hpp"


class Obstacle: public GameEntity
{
public:
  Obstacle()
    : GameEntity("Obstacle")
  {}

  Obstacle(int const & health,
           QVector2D const & position)
    :GameEntity(position, "Obstacle"),
      m_health(health)
  {}

  Obstacle(int const & health,
           QVector2D const & position,
           std::shared_ptr<QImage> image,
           TSize const & size)
    :GameEntity(position, "Obstacle", image, size),
      m_health(health)
  {}

  ~Obstacle() override;

  void Update() override;
  int GetHealth() const;
  void SetHealth(int const & health);

private:
  int m_health = 0;
};

using TObstaclePtr = std::shared_ptr<Obstacle>;

std::ostream & operator << (std::ostream & os,
                            Obstacle const & obj);
