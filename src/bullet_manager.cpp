#include "bullet_manager.hpp"

BulletManager::~BulletManager()
{

}

void BulletManager::Fire()
{
  if (!m_bulletList.empty())
  {
    m_bulletList.pop_front();
  }
}

std::list<TBulletPtr> BulletManager::GetBullets() const
{
  return m_bulletList;
}

void BulletManager::SetAmmo(uint const & countBullet)
{
  if (countBullet > 0)
  {
    m_bulletList = std::list<TBulletPtr>(countBullet, TBulletPtr(new Bullet()));
  }
}

std::ostream & operator << (std::ostream & os,
                            const BulletManager & obj)
{
  os << "BulletManager [";
  auto bullets = obj.GetBullets();
  for(TBulletPtr const & bullet : bullets)
  {
    os << *bullet;
  }
  os << "]";
  return os;
}
