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

int Alien::GetSpeed() const
{
  return m_speed;
}

void Alien::SetSpeed(int const & rate)
{
  m_speed = rate;
}

bool Alien::Shot()
{
  bool shot = false;
  m_shotTime--;
  if(m_shotTime == 0)
  {
    m_shotTime = m_frequency;
    shot = true;
  }
  return shot;
}

void Alien::IncreaseX(float const &value)
{
  float tmp = m_position.x() + value;

  // Set right wall.
  if (tmp > Globals::Width)
  {
    m_position.setX(m_position.x() - 10.0f);

    ReverseDirection();
  }
  else
  {
    m_position.setX(tmp);
  }
}

void Alien::DecreaseX(float const &value)
{

  // Set left wall.
  float tmp = m_position.x() - value;

  if (tmp < 0.0f)
  {
    m_position.setX(m_position.x() + 10.0f);

    ReverseDirection();
  }
  else
  {
    m_position.setX(tmp);
  }
}

void Alien::ReverseDirection()
{
  SetSpeed(-GetSpeed());
}

int Alien::GetAbsoluteSpeed() const
{
  return abs(m_speed);
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
