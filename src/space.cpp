#include "space.hpp"
#include "constants.hpp"

const std::list<TAlienPtr> &Space::GetAliens() const
{
  return m_alienList;
}

void Space::AddAlien(TAlienPtr alien)
{
  m_alienList.push_back(alien);
}

const std::list<TObstaclePtr> &Space::GetObstacles() const
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

const std::list<TBulletPtr> &Space::GetAlienBullets() const
{
  return m_alienBulletList;
}

void Space::AddAlienBullet(TBulletPtr bullet)
{
  m_alienBulletList.push_back(bullet);
}

const std::list<TBulletPtr> & Space::GetSpaceShipBullets() const
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

Space::~Space()
{

}

std::ostream & operator << (std::ostream & os,
                            const Space & obj)
{
  os << "Space";
  return os;
}
