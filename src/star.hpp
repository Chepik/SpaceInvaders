#pragma once

#include "game_entity.hpp"

class Star : public GameEntity
{
public:
  Star()
    : GameEntity("Star")
  {}

  Star(QVector2D const & position)
    : GameEntity(position, "Star")
  {}

  Star(QVector2D const & position,
       std::shared_ptr<QImage> image)
    : GameEntity(position, "Star", image)
  {}

  ~Star() override;

  void Update() override;

private:

};
