#include "images.hpp"

void Images::LoadImages()
{
  m_imageAlien = std::make_shared<QImage>(
        "data/alien.png");

  m_imageStar = std::make_shared<QImage>(
        "data/star.png");

  m_imageSpaceShip = std::make_shared<QImage>(
        "data/space_ship.png");

  m_imageObstacle = std::make_shared<QImage>(
        "data/obstacle.png");

  m_imageBullet = std::make_shared<QImage>(
        "data/bullet.png");

  m_imageExplosion= std::make_shared<QImage>(
        "data/explosion.png");

  m_imageBulletAlien= std::make_shared<QImage>(
      "data/bullet_alien.png");
}

std::shared_ptr<QImage> Images::GetImageAlien()
{
  return m_imageAlien;
}

std::shared_ptr<QImage> Images::GetImageStar()
{
  return m_imageStar;
}

std::shared_ptr<QImage> Images::GetImageSpaceShip()
{
  return m_imageSpaceShip;
}

std::shared_ptr<QImage> Images::GetImageObstacle()
{
  return m_imageObstacle;
}

std::shared_ptr<QImage> Images::GetImageBullet()
{
  return m_imageBullet;
}

std::shared_ptr<QImage> Images::GetImageExplosion()
{
  return m_imageExplosion;
}
std::shared_ptr<QImage> Images::GetImageBulletAlien()
{
  return m_imageBulletAlien;
}
