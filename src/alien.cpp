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

bool Alien::Shot()
{
  bool shot = false;
  m_shotTime--;
  if(m_shotTime == 0)
  {
    m_shotTime = m_rate;
    shot = true;
  }
  return shot;
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
