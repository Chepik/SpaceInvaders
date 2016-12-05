#include <stdexcept>
#include "except.hpp"
#include "obstacle.hpp"


Obstacle::~Obstacle()
{

}

uint Obstacle::GetHealth() const
{
  return m_health;
}

void Obstacle::SetHealth(uint const & health)
{
  m_health = health;
}

void Obstacle::Update()
{
  throw NotImplementedException();
}

std::ostream & operator << (std::ostream & os,
                            const Obstacle & obj)
{
//  os << "Obstacle [Position: " << obj.GetPosition()
//     << "; Health: " << obj.GetHealth() << "]";
  return os;
}
