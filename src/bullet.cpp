#include "except.hpp"
#include "bullet.hpp"


Bullet::~Bullet()
{

}

uint Bullet::GetDamage() const
{
  return m_damage;
}

void Bullet::SetDamage(uint const & damage)
{
  m_damage = damage;
}

void Bullet::Update()
{
  throw NotImplementedException();
}

void Bullet::Move()
{
  throw NotImplementedException();
}
void Bullet::IncreaseY(float const & value) {
  m_position.setY(m_position.y() + value);
}

std::ostream & operator << (std::ostream & os,
                            const Bullet & obj)
{  
//  os << "Bullet [Position: "
//     << obj.GetPosition().x() << " "
//     << obj.GetPosition().y()
//     << "; Damage: " << obj.GetDamage() << "]";
  return os;
}
