#include "images.hpp"

#include <QDebug>

#include "except.hpp"

void Images::LoadImages()
{
  std::string imageAlienPath = "data/alien.png";
  std::string imageStarPath = "data/star.png";
  std::string imageSpaceShipPath = "data/space_ship.png";
  std::string imageObstaclePath = "data/obstacle.png";
  std::string imageBulletPath = "data/bullet.png";
  std::string imageBulletAlienPath = "data/bullet_alien.png";
  std::string imageExplosionPath = "data/explosion.png";

  m_imageAlien = LoadImage(imageAlienPath);
  m_imageStar = LoadImage(imageStarPath);
  m_imageSpaceShip = LoadImage(imageSpaceShipPath);
  m_imageObstacle = LoadImage(imageObstaclePath);
  m_imageBullet = LoadImage(imageBulletPath);
  m_imageBulletAlien = LoadImage(imageBulletAlienPath);
  m_imageExplosion = LoadImage(imageExplosionPath);
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

std::shared_ptr<QImage> Images::LoadImage(std::string path)
{
  auto image = std::make_shared<QImage>(path.c_str());

  if (image->isNull())
  {
    throw CantLoadImagesException(path);
  }

  return image;
}

std::shared_ptr<QImage> Images::GetImageBulletAlien()
{
  return m_imageBulletAlien;
}
