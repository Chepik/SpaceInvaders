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

//QOpenGLTexture *Bullet::GetTexture() const
//{
//  return m_texture;
//}

//const QVector2D & Bullet::GetPos() const
//{
//  return m_pos;
//}

//void Bullet::IncreaseY(const float & value)
//{
//  m_pos.setY(m_pos.y() + value);
//}

void Bullet::Update()
{
  throw NotImplementedException();
}

void Bullet::Move()
{
  throw NotImplementedException();
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
