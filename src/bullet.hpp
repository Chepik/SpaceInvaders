#include "game_entity.hpp"
#include "movable_interface.hpp"

#include <QVector2D>
#include <QOpenGLTexture>

class Bullet: public GameEntity, public MovableInterface
{
public:
  Bullet()
    : GameEntity("Bullet")
  {}
  
  Bullet(uint const & damage, Point2D const & position)
    :GameEntity(position, "Bullet"),
      m_damage(damage)
  {}

  Bullet(uint const & damage,
         const QVector2D & pos,
         QImage * image) :
    m_damage(damage), m_pos(pos)
  {
    m_texture = new QOpenGLTexture(*image);
  }

  ~Bullet() override;

  void Update() override;
  void Move() override;
  uint GetDamage() const;
  void SetDamage(uint const & damage);
  QOpenGLTexture * GetTexture() const;
  const QVector2D & GetPos() const;
  void IncreaseY(float const & value);

private:
  uint m_damage = 0;

  QVector2D m_pos;

  QOpenGLTexture * m_texture = nullptr;
};

using TBulletPtr = std::shared_ptr<Bullet>;

std::ostream & operator << (std::ostream & os,
                            Bullet const & obj);
