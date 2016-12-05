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
       std::shared_ptr<QImage> image,
       Box2D const & size)
    : GameEntity(position, "Star", image, size)
  {}

  ~Star() override;

  void Update() override;

private:

};

using TStarPtr = std::shared_ptr<Star>;
