#include "game_entity.hpp"

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
  m_position.setY(m_position.y() + value);
}

GameEntity::~GameEntity()
{

}
