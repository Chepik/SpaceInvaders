#pragma once

#include "game_entity_with_weapon.hpp"

class SpaceShip: public GameEntityWithWeapon
{
public:
  SpaceShip()
    : GameEntityWithWeapon("SpaceShip")
  {}

  SpaceShip(QVector2D const & position,
            uint const & rate,
            uint const & health)
    : GameEntityWithWeapon(position,"SpaceShip", rate, health)
  {}

  SpaceShip(QVector2D const & position,
            uint const & rate,
            uint const & health,
            QImage * image)
    : GameEntityWithWeapon(position,"SpaceShip", rate, health, image)
  {}

  ~SpaceShip() override;

  void Update() override;
  void Move() override;
};

std::ostream & operator << (std::ostream & os,
                            SpaceShip const & obj);
