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
#include "game_state.hpp"

class GameWindow;

double constexpr PI = 3.14159265;

QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)
QT_FORWARD_DECLARE_CLASS(QOpenGLShader)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

struct RandomStar
{
  std::pair<float,float> m_randomStar;
  float m_periodStar;
};

class GLWidget : public QGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT
signals:
  void gameOver(GameState gameState, size_t score);

public:
  GLWidget(GameWindow * parent,
           QColor const & background,
           size_t const & level = 1);
  ~GLWidget();

protected:
  ///
  /// It needs to set focus for the widget.
  ///
  void showEvent(QShowEvent * event) override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void initializeGL() override;

  void Update(float elapsedSeconds);

  /// Set m_isGameOver to true if game is over.
  void IsGameOver();

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
  void RenderStar();
  void RenderExplosion();

  /// Logic stage.
  void CheckHitAlien();
  void CheckHitSpaceShip();
  void KillSpaceShip(uint damage, QVector2D const position);
  void SpaceShipBulletsLogic(float const & elapsedSeconds);
  void AlienBulletsLogic(float const & elapsedSeconds);
  void AlienLogic(float const & elapsedSeconds);
  void ShotAlien();
  void ExplosionLogic();
  void CheckHitObstacle();
  void StarLogic();
  void SetPosition(int w, int h);
  void CheckSpaceShipCollision();

  ///
  /// Generate random number between min and max values.
  ///
  /// Look here for more info:
  /// http://www.cplusplus.com/reference/random/uniform_real_distribution/
  ///
  float Random(float min, float max);

private:
  int L2D(int px) const { return px * devicePixelRatio(); }

  GameWindow * m_mainWindow;

  unsigned int m_frames = 0;
  QTime m_time;
  QTime m_timeFPS;
  QColor m_background;
  QSize m_screenSize;

  // The current level number.
  size_t m_level = 0;

  std::vector<RandomStar> m_random;

  std::shared_ptr<Space> m_space = nullptr;

  TexturedRect * m_texturedRect = nullptr;

  std::array<bool, 4> m_directions = {{ false, false, false, false }};

  std::default_random_engine m_generator;

  GameState m_gameState = GameState::STOP;

  size_t m_score = 0;
};
