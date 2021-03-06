#pragma once

#include <memory>
#include <QVector2D>
#include <QOpenGLTexture>

//#include "point2d.hpp"
#include "box2d.hpp"

class GameEntity 
{
public:
  GameEntity() = default;

  GameEntity(std::string const & name)
    : m_name(name)
  {}

  GameEntity(QVector2D const & position, std::string const & name)
    : m_position(position),
      m_name(name)
  {}

  GameEntity(QVector2D const & position,
             std::string const & name,
             std::shared_ptr<QImage> image,
             std::pair<int,int> const & size)
    : m_position(position),
      m_name(name),
      m_size(size)
  {
    m_texture = std::make_shared<QOpenGLTexture>(*image);
  }

  //TODO Add copy constructor!

  virtual ~GameEntity();

  std::string const & GetName() const;

  virtual void Update() = 0;

  QVector2D const & GetPosition() const;
  void SetPosition(QVector2D const & point);

  std::pair<int,int> const & GetSize() const;
  void SetSize(std::pair<int,int> const & size);

  std::shared_ptr<QOpenGLTexture> GetTexture();
  void SetTexture(std::shared_ptr<QOpenGLTexture> texture);

  virtual void IncreaseY(float const & value);
  virtual void DecreaseY(float const & value);

  virtual void IncreaseX(float const & value);
  virtual void DecreaseX(float const & value);
  
protected:
  QVector2D m_position;
  std::string m_name;
  std::shared_ptr<QOpenGLTexture> m_texture = nullptr;
  //Width and Heigth
  std::pair<int,int> m_size;
};

using TSize = std::pair<int, int>;
using TGameEntityPtr = std::shared_ptr<GameEntity>;
using TGameEntityConstPtr = std::shared_ptr<GameEntity const>;
