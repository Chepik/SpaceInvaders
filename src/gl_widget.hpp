#pragma once

#include <QGLWidget>
#include <QOpenGLFunctions>
#include <QTime>

#include <array>
#include <random>
#include <memory>

#include "textured_rect.hpp"
#include "bullet.hpp"
#include "space.hpp"
#include "unordered_map"
#include "images.hpp"
#include "explosion.hpp"

class GameWindow;

double constexpr PI = 3.14159265;

QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)
QT_FORWARD_DECLARE_CLASS(QOpenGLShader)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT
public:
  GLWidget(GameWindow * mw,
           QColor const & background,
           size_t const & level = 1);
  ~GLWidget();

protected:
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void initializeGL() override;

  void Update(float elapsedSeconds);


  /// Mouse and keyboard events.
  void mousePressEvent(QMouseEvent * e) override;
  void mouseDoubleClickEvent(QMouseEvent * e) override;
  void mouseMoveEvent(QMouseEvent * e) override;
  void mouseReleaseEvent(QMouseEvent * e) override;
  void wheelEvent(QWheelEvent * e) override;
  void keyPressEvent(QKeyEvent * e) override;
  void keyReleaseEvent(QKeyEvent * e) override;

  /// Create objects.
  void AddObstacles();
  void AddSpaceShip();
  void AddAliens();
  void AddStars();

  /// Render stage.
  void RenderAlien();
  void RenderSpaceShip();
  void RenderBullet();
  void RenderObstacle();
  void RenderStar(float blend);
  void RenderExplosion();

  /// Logic stage.
  void CheckHitAlien();
  void CheckHitSpaceShip();
  void KillSpaceShip(uint damage, QVector2D const position);
  void SpaceShipBulletsLogic();
  void AlienBulletsLogic();
  void AlienLogic();
  void ShotAlien();
  void ExplosionLogic();
  void ObstacleLogic();

  ///
  /// Generate random number between min and max values.
  ///
  /// Look here for more info:
  /// http://www.cplusplus.com/reference/random/uniform_real_distribution/
  ///
  double Random(double min, double max);

private:
  // Read current level parameters from a settings file.
  void ReadSettings();

  int L2D(int px) const { return px * devicePixelRatio(); }

  GameWindow * m_mainWindow;

  unsigned int m_frames = 0;
  QTime m_time;
  QColor m_background;
  QSize m_screenSize;

  // The current level number.
  size_t m_level = 0;

  // Number of aliens on the current level.
  size_t m_aliensNumber = 0;

  /// m_period is a value between 0.0 and 1.0 .
  /// It as a period.
  float m_period = 0.0f;

  std::vector<std::pair<float,float>> m_random;

  std::shared_ptr<Space> m_space = nullptr;

  TexturedRect * m_texturedRect = nullptr;

  std::array<bool, 4> m_directions = {{ false, false, false, false }};

  std::default_random_engine m_generator;
};
