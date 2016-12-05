#pragma once

#include <memory>
#include <QImage>

#include "singleton.h"

class Images : public Singleton<Images>
{
public:
  void LoadImages();

  std::shared_ptr<QImage> GetImageAlien();
  std::shared_ptr<QImage> GetImageStar();
  std::shared_ptr<QImage> GetImageSpaceShip();
  std::shared_ptr<QImage> GetImageObstacle();
  std::shared_ptr<QImage> GetImageBullet();

private:
  /// Otherwise it won't be accessible in parent class Singleton<Images>.
  friend class Singleton<Images>;

  Images() = default;

  std::shared_ptr<QImage> m_imageAlien = nullptr;
  std::shared_ptr<QImage> m_imageStar = nullptr;
  std::shared_ptr<QImage> m_imageSpaceShip = nullptr;
  std::shared_ptr<QImage> m_imageObstacle = nullptr;
  std::shared_ptr<QImage> m_imageBullet = nullptr;
};
