#include "space.hpp"
#include "constants.hpp"

std::list<TAlienPtr> &Space::GetAliens()
{
  return m_alienList;
}

void Space::AddAlien(TAlienPtr alien)
{
  m_alienList.push_back(alien);
}

std::list<TObstaclePtr> &Space::GetObstacles()
{
  return m_obstacleList;
}

void Space::AddObstacle(TObstaclePtr obstacle)
{
  m_obstacleList.push_back(obstacle);
}

const std::list<TStarPtr> &Space::GetStars() const
{
  return m_starList;
}

void Space::AddStar(TStarPtr star)
{
  m_starList.push_back(star);
}

std::list<TBulletPtr> &Space::GetAlienBullets()
{
  return m_alienBulletList;
}

void Space::AddAlienBullet(TBulletPtr bullet)
{
  m_alienBulletList.push_back(bullet);
}

std::list<TBulletPtr> & Space::GetSpaceShipBullets()
{
  return m_spaceShipBulletList;
}

void Space::AddSpaceShipBullet(TBulletPtr bullet)
{
  m_spaceShipBulletList.push_back(bullet);
}

const TSpaceShipPtr & Space::GetSpaceShip() const
{
  return m_space_ship;
}

void Space::SetSpaceShip(TSpaceShipPtr spaceShip)
{
  m_space_ship = spaceShip;
}

void Space::AddExplosion(TExplosionPtr explosion)
{
  m_explosionList.push_back(explosion);
}

Space::~Space()
{

}

std::ostream & operator << (std::ostream & os,
                            const Space & obj)
{
  os << "Space";
  return os;
}


std::list<TExplosionPtr> &Space::GetExplosions()
{
  return m_explosionList;
}
