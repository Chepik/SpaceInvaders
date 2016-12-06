#include "except.hpp"
#include "explosion.hpp"


Explosion::~Explosion()
{

}

bool Explosion::ReduceLifeTime()
{
  bool dead = false;
  m_lifetime--;
  if(m_lifetime == 0)
    dead = true;
  return dead;
}

void Explosion::Update()
{
  throw NotImplementedException();
}
//std::ostream & operator << (std::ostream & os,
//                            const Explosion & obj)
//{
//  return os;
//}
