#include "gl_widget.hpp"

#include <QPainter>
#include <QPaintEngine>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QCoreApplication>
#include <QtGui/QMouseEvent>
#include <QtGui/QGuiApplication>
#include <QDebug>

#include <cmath>
#include <iostream>

#include "json/assertions.h"
#include "json/value.h"
#include "json/writer.h"
#include "json/json.h"
#include "json/config.h"

#include "game_window.hpp"
#include "constants.hpp"
#include "util.hpp"

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

GLWidget::GLWidget(GameWindow * mw,
                   QColor const & background,
                   size_t const & level)
  : m_mainWindow(mw),
    m_background(background),
    m_level(level)
{
  m_space = std::make_shared<Space>();

  setMinimumSize(Globals::Width, Globals::Height);
  setFocusPolicy(Qt::StrongFocus);
}

GLWidget::~GLWidget()
{
  makeCurrent();

  doneCurrent();
}

void GLWidget::initializeGL()
{
  initializeOpenGLFunctions();

  m_texturedRect = new TexturedRect();
  m_texturedRect->Initialize(this);

  Images::Instance().LoadImages();

  // Read current level parameters from a settings file.
  try
  {
    Json::Value settings;

    settings = Util::ReadJson(Globals::SettingsFileName);

    m_aliensNumber = \
        settings["Level"][std::to_string(m_level)]["AliensNumber"].asUInt();

    qDebug() << "m_aliensNumber = " << m_aliensNumber;
  }
  catch(std::exception ex) {
    qDebug() << "Can't read settings from a file!";
  }

  // Create aliens.
  for (size_t i = 1; i <= m_aliensNumber; i++) {
    m_space->AddAlien(std::make_shared<Alien>(
                        100,
                        QVector2D(i * 200, 600),
                        100,
                        100,
                        Images::Instance().GetImageAlien(),
                        std::make_pair(128,128)));
  }

  // Create a space ship
  m_space->SetSpaceShip(std::make_shared<SpaceShip>(
                          QVector2D(200, 600),
                          100,
                          100,
                          Images::Instance().GetImageSpaceShip(),
                          std::make_pair(128,128)));

  // Create obstacles.
  m_space->AddObstacle(std::make_shared<Obstacle>(
                         100,
                         QVector2D(200, 600),
                         Images::Instance().GetImageObstacle(),
                         std::make_pair(128,128)));

  // Create stars.
  AddStar();

  m_time.start();
}

void GLWidget::paintGL()
{
  int const elapsed = m_time.elapsed();
  Update(elapsed / 1000.0f);

  QPainter painter;
  painter.begin(this);
  painter.beginNativePainting();

  glClearColor(m_background.redF(),
               m_background.greenF(),
               m_background.blueF(),
               1.0f);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glFrontFace(GL_CW);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  CheckHitAlien();

  SpaceShipBulletsLogic();

  AlienBulletsLogic();

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
  // transparency is a value between 0.0 and 1.0 .
  float transperancy = static_cast<float>(sin(m_period * 2 * PI));

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
  {
    m_space->GetSpaceShip()->IncreaseY(kSpeed * elapsedSeconds);
  }
  if (m_directions[kDownDirection])
  {
    m_space->GetSpaceShip()->DecreaseY(kSpeed * elapsedSeconds);
  }
  if (m_directions[kLeftDirection])
  {
    m_space->GetSpaceShip()->DecreaseX(kSpeed * elapsedSeconds);
  }
  if (m_directions[kRightDirection])
  {
    m_space->GetSpaceShip()->IncreaseX(kSpeed * elapsedSeconds);
  }
}

void GLWidget::Render()
{
  for (auto alien : m_space->GetAliens()) {
    m_texturedRect->Render(alien->GetTexture(),
                           alien->GetPosition(),
                           QSize(128, 128),
                           m_screenSize,
                           1.0);
  }
}

void GLWidget::RenderSpaceShip()
{
  m_texturedRect->Render(m_space->GetSpaceShip()->GetTexture(),
                         m_space->GetSpaceShip()->GetPosition(),
                         QSize(128, 128),
                         m_screenSize,
                         1.0);
}

void GLWidget::RenderBullet()
{
  for (auto bullet : m_space->GetSpaceShipBullets()) {
    m_texturedRect->Render(bullet->GetTexture(),
                           bullet->GetPosition(),
                           QSize(128, 128),
                           m_screenSize,
                           1.0);
  }
  for (auto bullet : m_space->GetAlienBullets()) {
    m_texturedRect->Render(bullet->GetTexture(),
                           bullet->GetPosition(),
                           QSize(128, 128),
                           m_screenSize,
                           1.0);
  }
}

void GLWidget::RenderObstacle()
{
  for (auto obstacle : m_space->GetObstacles()) {
    m_texturedRect->Render(obstacle->GetTexture(),
                           obstacle->GetPosition(),
                           QSize(128, 128),
                           m_screenSize,
                           1.0);
  }
}

void GLWidget::RenderStar(float blend)
{
  int i=0;

  for (auto it = m_space->GetStars().begin();
       it != m_space->GetStars().end() || i <m_random.size();
       ++it, i++)
  {
    m_texturedRect->Render(
          (*it)->GetTexture(),
          QVector2D(m_random.at(i).first*kWidth,
                    m_random.at(i).second*kHeight),
          QSize(16, 16),
          m_screenSize,
          blend);
  }
}

void GLWidget::AddStar()
{
  m_space->AddStar(std::make_shared<Star>(
                     QVector2D(200, 600),
                     Images::Instance().GetImageStar(),
                     std::make_pair(128,128)));

  m_random.push_back(std::make_pair(Random(0,1), Random(0,1)));
}

void GLWidget::CheckHitSpaceShip()
{
  QVector2D positionSpaceShip = m_space->GetSpaceShip()->GetPosition();

  TSize sizeSpaceShip = m_space->GetSpaceShip()->GetSize();

  Box2D spaceShipBox = Box2D::createBox(
        Point2D(positionSpaceShip.x(), positionSpaceShip.y()),
        Point2D(positionSpaceShip.x() + sizeSpaceShip.first,
                positionSpaceShip.y() + sizeSpaceShip.second));

  for (auto bullet : m_space->GetAlienBullets())
  {
    QVector2D position = bullet->GetPosition();

    TSize size = bullet->GetSize();

    Box2D bulletBox = Box2D::createBox(
          Point2D(position.x(), position.y()),
          Point2D(position.x() + size.first,
                  position.y() + size.second));

    // If two boxes are not intersected with each other
    // then return false.
    if (Box2D::checkBoxes(spaceShipBox,bulletBox))
    {
      KillSpaceShip(bullet->GetDamage());
    }
  }
}

void GLWidget::KillSpaceShip(uint damage)
{
  uint health = m_space->GetSpaceShip()->GetHealth();

  if ((health-damage) > 0)
  {
    m_space->GetSpaceShip()->SetHealth(health-damage);
  }
  else
  {
    //emit signal Game Over
    qDebug() << "Game Over!!!!!!!!!!!!!!!!!!!!!!!!!";
  }
}

void GLWidget::CheckHitAlien()
{
  std::list<TBulletPtr> & lstBullet = m_space->GetSpaceShipBullets();

  std::list<TAlienPtr> & lstAlien = m_space->GetAliens();

  for (auto itAlien = begin(lstAlien); itAlien != end(lstAlien);)
  {
    QVector2D positionAlien = (*itAlien)->GetPosition();

    TSize sizeAlien = (*itAlien)->GetSize();

    Box2D alienBox = Box2D::createBox(
          Point2D(positionAlien.x(), positionAlien.y()),
          Point2D(positionAlien.x() + sizeAlien.first,
                  positionAlien.y() + sizeAlien.second));

    bool flag = false;

    for (auto it = begin(lstBullet); it != end(lstBullet); ++it)
    {
      QVector2D position = (*it)->GetPosition();

      TSize size = (*it)->GetSize();

      Box2D bulletBox = Box2D::createBox(
            Point2D(position.x(), position.y()),
            Point2D(position.x() + size.first,
                    position.y() + size.second));

      // If two boxes are not intersected with each other
      // then return false.
      if (Box2D::checkBoxes(alienBox,bulletBox))
      {
        it = lstBullet.erase(it);
        flag = true;
        break;
      }
    }
    if (flag)
    {
      itAlien = lstAlien.erase(itAlien);
    }
    else
    {
      ++itAlien;
    }
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
  {
    m_directions[kUpDirection] = true;
  }
  else if (e->key() == Qt::Key_Down)
  {
    m_directions[kDownDirection] = true;
  }
  else if (e->key() == Qt::Key_Left)
  {
    m_directions[kLeftDirection] = true;
  }
  else if (e->key() == Qt::Key_Right)
  {
    m_directions[kRightDirection] = true;
  }
  else if (e->key() == Qt::Key_Space)
  {
    std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(
          m_space->GetSpaceShip()->GetPosition(),
          Images::Instance().GetImageBullet(),
          100,
          std::make_pair(128,128));

    m_space->AddSpaceShipBullet(bullet);
  }
}

void GLWidget::keyReleaseEvent(QKeyEvent * e)
{
  if (e->key() == Qt::Key_Up)
  {
    m_directions[kUpDirection] = false;
  }
  else if (e->key() == Qt::Key_Down)
  {
    m_directions[kDownDirection] = false;
  }
  else if (e->key() == Qt::Key_Left)
  {
    m_directions[kLeftDirection] = false;
  }
  else if (e->key() == Qt::Key_Right)
  {
    m_directions[kRightDirection] = false;
  }
}

double GLWidget::Random(double min, double max)
{
  std::uniform_real_distribution<double> distribution(min, max);

  double number = distribution(m_generator);

  return number;
}

void GLWidget::SpaceShipBulletsLogic()
{
  // Loop over space ship bullets and delete it if needed.
  std::list<TBulletPtr> & lst = m_space->GetSpaceShipBullets();

  for (auto it = begin(lst); it != end(lst);)
  {
    (*it)->IncreaseY(100);

    if ((*it)->GetPosition().y() > Globals::Height)
    {
      it = lst.erase(it);
    }
    else
    {
      ++it;
    }
  }

  // Check if needed.
  //  qDebug() << "lst.size() = " << lst.size();
}
void GLWidget::AlienBulletsLogic()
{
  // Loop over space ship bullets and delete it if needed.
  std::list<TBulletPtr> & lst = m_space->GetAlienBullets();

  for (auto it = begin(lst); it != end(lst);)
  {
    (*it)->DecreaseX(100);

    if ((*it)->GetPosition().y() < 0.0f)
    {
      it = lst.erase(it);
    }
    else
    {
      ++it;
    }
  }

  // Check if needed.
  //  qDebug() << "lst.size() = " << lst.size();
}
