#include "game_entity.hpp"

GameEntity::~GameEntity() {}

std::string const & GameEntity::GetName() const
{
  return m_name;
}

QVector2D const & GameEntity::GetPosition() const
{
  return m_position;
}

void GameEntity::SetPosition(QVector2D const & point)
{
  m_position = point;
}

const TSize & GameEntity::GetSize() const
{
  return m_size;
}

void GameEntity::SetSize(const TSize & size)
{
  m_size = size;
}

std::shared_ptr<QOpenGLTexture> GameEntity::GetTexture()
{
  return m_texture;
}

void GameEntity::SetTexture(
    std::shared_ptr<QOpenGLTexture> texture)
{
  m_texture = texture;
}

void GameEntity::IncreaseY(const float & value)
{
  float tmp = m_position.y() + value;

  // Set top wall.
  if (tmp > Globals::Height)
  {
    m_position.setY(m_position.y() - 10.0f);
  }
  else
  {
    m_position.setY(tmp);
  }
}

void GameEntity::DecreaseY(float const & value)
{
  float tmp = m_position.y() - value;

  // Set bottom wall.
  if (tmp < 0.0f)
  {
    m_position.setY(m_position.y() + 10.0f);
  }
  else
  {
    m_position.setY(tmp);
  }
}

void GameEntity::IncreaseX(float const &value)
{
  float tmp = m_position.x() + value;

  // Set right wall.
  if (tmp > Globals::Width)
  {
    m_position.setX(m_position.x() - 10.0f);
  }
  else
  {
    m_position.setX(tmp);
  }
}

void GameEntity::DecreaseX(float const &value)
{

  // Set left wall.
  float tmp = m_position.x() - value;

  if (tmp < 0.0f)
  {
    m_position.setX(m_position.x() + 10.0f);
  }
  else
  {
    m_position.setX(tmp);
  }
}
