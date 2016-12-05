#include "except.hpp"
#include "alien.hpp"


Alien::~ Alien()
{

}

void Alien::Move()
{
  throw NotImplementedException();
}

void Alien::Update()
{
  throw NotImplementedException();
}

uint Alien::GetSpeed() const
{
  return m_speed;
}

void Alien::SetSpeed(uint const & rate)
{
  m_speed = rate;
}

std::ostream & operator << (std::ostream & os,
                            const Alien & obj)
{
//  os << "Alien [Position: " << obj.GetPosition()
//     << "; Rate: " << obj.GetRate()
//     << "; Health: " << obj.GetHealth()
//     << "; Speed: " << obj.GetSpeed() << "]";
  return os;
}
