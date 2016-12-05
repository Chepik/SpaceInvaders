#pragma once

#include <list>
#include "alien.hpp"
#include "space_ship.hpp"
#include "obstacle.hpp"
#include "star.hpp"

class Space
{
public:
  Space() = default;
  virtual ~Space();
  std::list<TAlienPtr> const & GetAliens() const;
  void AddAlien(TAlienPtr alien);
  std::list<TObstaclePtr> const & GetObstacles() const;
  void AddObstacle(TObstaclePtr obstacle);
  std::list<TStarPtr> const & GetStars() const;
  void AddStar(TStarPtr star);
  std::list<TBulletPtr> const & GetAlienBullets() const;
  void AddAlienBullet(TBulletPtr bullet);
  std::list<TBulletPtr> const & GetSpaceShipBullets() const;
  void AddSpaceShipBullet(TBulletPtr bullet);
  TSpaceShipPtr const & GetSpaceShip() const;
  void SetSpaceShip(TSpaceShipPtr spaceShip);
  
private:
  std::list<TAlienPtr> m_alienList;
  TSpaceShipPtr m_space_ship = nullptr;
  std::list<TObstaclePtr> m_obstacleList;
  std::list<TStarPtr> m_starList;
  std::list<TBulletPtr> m_spaceShipBulletList;
  std::list<TBulletPtr> m_alienBulletList;

};

std::ostream & operator << (std::ostream & os,
                            Space const & obj);
