#include "except.hpp"
#include "space_ship.hpp"


void SpaceShip::Update()
{
  throw NotImplementedException();
}

void SpaceShip::Move()
{
  throw NotImplementedException();
}

SpaceShip::~SpaceShip()
{

}

std::ostream & operator << (std::ostream & os,
                            const SpaceShip & obj)
{
//  os << "SpaceShip [Position: " << obj.GetPosition()
//     << "; Rate: " << obj.GetRate()
//     << "; Health: " << obj.GetHealth() << "]";
  return os;
}
