#include "gl_widget.hpp"

#include <QPainter>
#include <QPaintEngine>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QCoreApplication>
#include <QtGui/QMouseEvent>
#include <QtGui/QGuiApplication>
#include <cmath>

#include <iostream>

#include "game_window.hpp"

namespace
{

int constexpr kLeftDirection = 0;
int constexpr kRightDirection = 1;
int constexpr kUpDirection = 2;
int constexpr kDownDirection = 3;
int constexpr kWidth = 1024;
int constexpr kHeight = 768;

bool IsLeftButton(Qt::MouseButtons buttons)
{
  return buttons & Qt::LeftButton;
}
bool IsLeftButton(QMouseEvent const * const e)
{
  return IsLeftButton(e->button()) || IsLeftButton(e->buttons());
}

bool IsRightButton(Qt::MouseButtons buttons)
{
  return buttons & Qt::RightButton;
}
bool IsRightButton(QMouseEvent const * const e)
{
  return IsRightButton(e->button()) || IsRightButton(e->buttons());
}

} // namespace

GLWidget::GLWidget(GameWindow * mw, QColor const & background)
  : m_mainWindow(mw)
  , m_background(background)
{
  m_space = std::make_shared<Space>(kWidth, kHeight);

  setMinimumSize(kWidth, kHeight);
  setFocusPolicy(Qt::StrongFocus);
}

GLWidget::~GLWidget()
{
  makeCurrent();
  //  delete m_textureAlien;
  //  delete m_textureStar;
  //  delete m_textureSpaceShip;
  //  delete m_textureObstacle;
  //  delete m_texturedRect;
  doneCurrent();
}

void GLWidget::initializeGL()
{
  initializeOpenGLFunctions();

  m_texturedRect = new TexturedRect();
  m_texturedRect->Initialize(this);

  Images::Instance().LoadImages();

  m_alien = std::make_shared<Alien>(
        100,
        QVector2D(200, 600),
        100,
        100,
        Images::Instance().GetImageAlien());

  m_space_ship = std::make_shared<SpaceShip>(
        QVector2D(200, 600),
        100,
        100,
        Images::Instance().GetImageSpaceShip());

  m_obstacle = std::make_shared<Obstacle>(
        100,
        QVector2D(200, 600),
        Images::Instance().GetImageObstacle());

  m_star = std::make_shared<Star>(
        QVector2D(200, 600),
        Images::Instance().GetImageStar());

  m_time.start();
}

void GLWidget::paintGL()
{
  int const elapsed = m_time.elapsed();
  Update(elapsed / 1000.0f);

  QPainter painter;
  painter.begin(this);
  painter.beginNativePainting();

  glClearColor(m_background.redF(), m_background.greenF(), m_background.blueF(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glFrontFace(GL_CW);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  for (auto bullet : m_bulletList) {
    bullet->IncreaseY(100);
  }

  Render();

  RenderSpaceShip();

  RenderBullet();

  RenderObstacle();

  /// Set to zero if it reaches the 1.0 .
  if (m_period < 1.0)
  {
    m_period += 0.001f;
  }
  else
  {
    m_period = 0.0f;
    for (auto it = m_random.begin() ; it != m_random.end(); ++it)
    {
      *it = std::make_pair(Random(0,1), Random(0,1));
    }
  }

  // Generate a parameter for a star.
  // transperancy is a value between 0.0 and 1.0 .
  float transperancy = sin(m_period * 2 * PI);

  RenderStar(transperancy);
  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);
  painter.endNativePainting();

  if (elapsed != 0)
  {
    QString framesPerSecond;
    framesPerSecond.setNum(m_frames / (elapsed / 1000.0), 'f', 2);
    painter.setPen(Qt::white);
    painter.drawText(20, 40, framesPerSecond + " fps");
  }
  painter.end();

  if (!(m_frames % 100))
  {
    m_time.start();
    m_frames = 0;
  }
  ++m_frames;
  update();
}

void GLWidget::resizeGL(int w, int h)
{
  m_screenSize.setWidth(w);
  m_screenSize.setHeight(h);
}

void GLWidget::Update(float elapsedSeconds)
{
  float const kSpeed = 20.0f; // pixels per second.

  if (m_directions[kUpDirection])
    m_position.setY(m_position.y() + kSpeed * elapsedSeconds);
  if (m_directions[kDownDirection])
    m_position.setY(m_position.y() - kSpeed * elapsedSeconds);
  if (m_directions[kLeftDirection])
    m_position.setX(m_position.x() - kSpeed * elapsedSeconds);
  if (m_directions[kRightDirection])
    m_position.setX(m_position.x() + kSpeed * elapsedSeconds);
}

void GLWidget::Render()
{
  m_texturedRect->Render(m_alien->GetTexture(),
                         QVector2D(200, 600),
                         QSize(128, 128),
                         m_screenSize,
                         1.0);

  m_texturedRect->Render(m_alien->GetTexture(),
                         QVector2D(400, 600),
                         QSize(128, 128),
                         m_screenSize,
                         1.0);

  m_texturedRect->Render(m_alien->GetTexture(),
                         QVector2D(600, 600),
                         QSize(128, 128),
                         m_screenSize,
                         1.0);
}

void GLWidget::RenderSpaceShip()
{
  m_texturedRect->Render(m_space_ship->GetTexture(),
                         m_position,
                         QSize(128, 128),
                         m_screenSize,
                         1.0);
}

void GLWidget::RenderBullet()
{
  for (auto bullet : m_bulletList) {
    m_texturedRect->Render(bullet->GetTexture(),
                           bullet->GetPosition(),
                           QSize(128, 128),
                           m_screenSize,
                           1.0);
  }
}

void GLWidget::RenderObstacle()
{
  m_texturedRect->Render(m_obstacle->GetTexture(),
                         QVector2D(500, 300),
                         QSize(128, 128),
                         m_screenSize,
                         1.0);
}

void GLWidget::RenderStar(float blend)
{
  for (auto it = m_random.begin() ; it != m_random.end(); ++it) {
    m_texturedRect->Render(
          m_star->GetTexture(),
          QVector2D((*it).first*kWidth, (*it).second*kHeight),
          QSize(16, 16),
          m_screenSize,
          blend);
  }
}

void GLWidget::mousePressEvent(QMouseEvent * e)
{
  QGLWidget::mousePressEvent(e);

  int const px = L2D(e->x());
  int const py = L2D(e->y());
  if (IsLeftButton(e))
  {
    // ...
  }
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent * e)
{
  QGLWidget::mouseDoubleClickEvent(e);

  int const px = L2D(e->x());
  int const py = L2D(e->y());
  if (IsRightButton(e))
  {
    // ...
  }
}

void GLWidget::mouseMoveEvent(QMouseEvent * e)
{
  QGLWidget::mouseMoveEvent(e);

  int const px = L2D(e->x());
  int const py = L2D(e->y());
  if (IsLeftButton(e))
  {
    // ...
  }
}

void GLWidget::mouseReleaseEvent(QMouseEvent * e)
{
  QGLWidget::mouseReleaseEvent(e);

  int const px = L2D(e->x());
  int const py = L2D(e->y());
  if (IsLeftButton(e))
  {
    // ...
  }
}

void GLWidget::wheelEvent(QWheelEvent * e)
{
  QGLWidget::wheelEvent(e);

  int const delta = e->delta();
  int const px = L2D(e->x());
  int const py = L2D(e->y());
  // ...
}

void GLWidget::keyPressEvent(QKeyEvent * e)
{
  if (e->key() == Qt::Key_Up)
    m_directions[kUpDirection] = true;
  else if (e->key() == Qt::Key_Down)
    m_directions[kDownDirection] = true;
  else if (e->key() == Qt::Key_Left)
    m_directions[kLeftDirection] = true;
  else if (e->key() == Qt::Key_Right)
    m_directions[kRightDirection] = true;
  else if (e->key() == Qt::Key_Space)
  {
    std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(
          QVector2D(200, 600), Images::Instance().GetImageBullet(), 100);

    m_bulletList.push_back(bullet);
  }
}

void GLWidget::keyReleaseEvent(QKeyEvent * e)
{
  if (e->key() == Qt::Key_Up)
    m_directions[kUpDirection] = false;
  else if (e->key() == Qt::Key_Down)
    m_directions[kDownDirection] = false;
  else if (e->key() == Qt::Key_Left)
    m_directions[kLeftDirection] = false;
  else if (e->key() == Qt::Key_Right)
    m_directions[kRightDirection] = false;
}

double GLWidget::Random(double min, double max)
{
  //    return (double)(rand())/RAND_MAX*(max - min) + min;

  std::uniform_real_distribution<double> distribution(min, max);

  double number = distribution(m_generator);

  return number;
}
