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
#include "logger.hpp"
#include "except.hpp"
#include "singleton.h"
#include "settings.hpp"

namespace
{

int constexpr kLeftDirection = 0;
int constexpr kRightDirection = 1;
int constexpr kUpDirection = 2;
int constexpr kDownDirection = 3;

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

GLWidget::GLWidget(GameWindow * parent,
                   QColor const & background,
                   size_t const & level)
  : m_mainWindow(parent),
    m_background(background),
    m_level(level)
{
  m_space = std::make_shared<Space>();

  setMinimumSize(Globals::Width, Globals::Height);
  setFocusPolicy(Qt::StrongFocus);

  connect(this, SIGNAL(gameOver(QString)),
          parent, SLOT(gameOver(QString)));
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

  std::string level = std::to_string(m_level);

  try
  {
    Images::Instance().LoadImages();
  }
  catch (LoadImagesException const & ex)
  {
    qDebug() << ex.what();

    throw InitialiseGameException();
  }

  try
  {
    Settings::Instance().LoadMainSettings();
    Settings::Instance().LoadLevelSettings(level);
  }
  catch (ReadSettingsException const & ex)
  {
    qDebug() << ex.what();

    throw InitialiseGameException();
  }

  AddAliens();

  AddSpaceShip();

  AddObstacles();

  AddStars();

  m_time.start();
}

void GLWidget::AddAliens()
{
  size_t aliensNumber = Settings::Instance().m_alienParameters.m_number;
  int speed = Settings::Instance().m_alienParameters.m_speed;
  uint health = Settings::Instance().m_alienParameters.m_health;
  TSize size = Settings::Instance().m_alienParameters.m_size;
  size_t aliensRowNumber = Settings::Instance().m_alienParameters.m_rowNumber;
  uint frequency = Settings::Instance().m_alienParameters.m_frequency;

  int height = size.second;

  size_t r = (Globals::Width / aliensNumber);

  for (size_t j = 0; j < aliensRowNumber; j++)
  {
    for (size_t i = 0; i < aliensNumber; i++)
    {
      m_space->AddAlien(std::make_shared<Alien>(
          speed,
          QVector2D(i * r, 600 + j*height),
          Settings::Instance().m_alienParameters.m_rate,
          health,
          Images::Instance().GetImageAlien(),
          size,
          frequency));
    }
  }
}

void GLWidget::AddSpaceShip()
{  
  uint health = Settings::Instance().m_spaceShipParameters.m_health;
  uint rate = Settings::Instance().m_spaceShipParameters.m_rate;
  TSize size = Settings::Instance().m_spaceShipParameters.m_size;

  m_space->SetSpaceShip(std::make_shared<SpaceShip>(
                          QVector2D(Globals::Width / 2, size.second),
                          rate,
                          health,
                          Images::Instance().GetImageSpaceShip(),
                          size));
}

void GLWidget::AddObstacles()
{  
  size_t obstaclesNumber = Settings::Instance().m_obstacleParameters.m_number;
  uint health = Settings::Instance().m_obstacleParameters.m_health;
  TSize size = Settings::Instance().m_obstacleParameters.m_size;

  size_t width = Settings::Instance().m_obstacleParameters.m_width;

  size_t r = (Globals::Width / obstaclesNumber);

  for (size_t i = 0; i < obstaclesNumber; i++)
  {
    m_space->AddObstacle(std::make_shared<Obstacle>(
                           health,
                           QVector2D(i*r + width, 300),
                           Images::Instance().GetImageObstacle(),
                           size));
  }
}

void GLWidget::paintGL()
{
  // Get time.
  int const elapsedMilliseconds = m_time.elapsed();
  int const elapsedMillisecondsFPS = m_timeFPS.elapsed();

  // Convert from milliseconds to seconds.
  float const elapsedSeconds = elapsedMilliseconds / 1000.0f;
  float const elapsedSecondsFPS = elapsedMillisecondsFPS / 1000.0f;

  qDebug() << "elapsedSeconds = " << elapsedSeconds;

  Update(elapsedSeconds);

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

  IsGameOver();

  ExplosionLogic();

  // It throws an error.
  CheckHitSpaceShip();

  AlienLogic(elapsedSeconds);

  CheckHitAlien();

  ShotAlien();

  SpaceShipBulletsLogic(elapsedSeconds);

  AlienBulletsLogic(elapsedSeconds);

  CheckHitObstacle();

  CheckSpaceShipCollision();

  RenderAlien();

  RenderSpaceShip();

  RenderBullet();

  RenderObstacle();

  RenderExplosion();

  /// Set to zero if it reaches the 1.0 .

  StarLogic();

  RenderStar();

  // Free the resources.
  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);
  painter.endNativePainting();

  // Print FPS to the screen.
  if (elapsedMilliseconds != 0)
  {
    QString framesPerSecond;
    framesPerSecond.setNum(m_frames / elapsedSecondsFPS, 'f', 2);
    painter.setPen(Qt::white);
    painter.drawText(20, 40, framesPerSecond + " fps");
  }
  painter.end();

  // Restart main timer.
  m_time.start();

  if (!(m_frames % 100))
  {
    // Restart FPS timer.
    m_timeFPS.start();

    m_frames = 0;
  }

  ++m_frames;

  if (!m_isGameOver)
  {
    update();
  }
  else
  {
    emit gameOver("Congratulations! You win!");

    return;
  }
}

void GLWidget::resizeGL(int w, int h)
{
  SetPosition(w, h);
  m_screenSize.setWidth(w);
  Globals::Width = w;
  m_screenSize.setHeight(h);
  Globals::Height = h;
}

void GLWidget::Update(float elapsedSeconds)
{
  float const kSpeed = Settings::Instance().m_spaceShipParameters.m_speed; // pixels per second.

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

void GLWidget::IsGameOver()
{
  std::list<TAlienPtr> & lstAlien = m_space->GetAliens();

  if (lstAlien.empty())
  {
    m_isGameOver = true;
  }
}

void GLWidget::RenderAlien()
{
  for (auto alien : m_space->GetAliens())
  {
    m_texturedRect->Render(alien->GetTexture(),
                           alien->GetPosition(),
                           alien->GetSize(),
                           m_screenSize,
                           1.0);
  }
}

void GLWidget::RenderSpaceShip()
{
  m_texturedRect->Render(m_space->GetSpaceShip()->GetTexture(),
                         m_space->GetSpaceShip()->GetPosition(),
                         m_space->GetSpaceShip()->GetSize(),
                         m_screenSize,
                         1.0);
}

void GLWidget::RenderBullet()
{
  for (auto bullet : m_space->GetSpaceShipBullets())
  {
    m_texturedRect->Render(bullet->GetTexture(),
                           bullet->GetPosition(),
                           bullet->GetSize(),
                           m_screenSize,
                           1.0);
  }
  for (auto bullet : m_space->GetAlienBullets())
  {
    m_texturedRect->Render(bullet->GetTexture(),
                           bullet->GetPosition(),
                           bullet->GetSize(),
                           m_screenSize,
                           1.0);
  }
}

void GLWidget::RenderObstacle()
{
  for (auto obstacle : m_space->GetObstacles())
  {
    m_texturedRect->Render(obstacle->GetTexture(),
                           obstacle->GetPosition(),
                           obstacle->GetSize(),
                           m_screenSize,
                           1.0);
  }
}

void GLWidget::RenderStar()
{
  int i=0;

  for (auto it = m_space->GetStars().begin();
       it != m_space->GetStars().end() || i <m_random.size();
       ++it, i++)
  {
    float blend = static_cast<float>(sin(m_random.at(i).m_periodStar * 2 * PI));
    m_texturedRect->Render(
          (*it)->GetTexture(),
          QVector2D(m_random.at(i).m_randomStar.first*Globals::Width,
                    m_random.at(i).m_randomStar.second*Globals::Height),
          (*it)->GetSize(),
          m_screenSize,
          blend);
  }
}

void GLWidget::RenderExplosion()
{
  for (auto explosion : m_space->GetExplosions())
  {
    m_texturedRect->Render(explosion->GetTexture(),
                           explosion->GetPosition(),
                           explosion->GetSize(),
                           m_screenSize,
                           1.0);
  }
}

void GLWidget::AddStars()
{
  size_t starsNumber = Settings::Instance().m_starParameters.m_number;
  TSize size = Settings::Instance().m_starParameters.m_size;

  for (size_t i = 1; i <= starsNumber; i++)
  {
    m_space->AddStar(std::make_shared<Star>(
                       QVector2D(200, 600),
                       Images::Instance().GetImageStar(),
                       size));

    RandomStar randomStar;
    randomStar.m_periodStar = Random(0, 1);
    randomStar.m_randomStar = std::make_pair(Random(0, 1), Random(0, 1));
    m_random.push_back(randomStar);
  }
}

void GLWidget::CheckHitSpaceShip()
{
  QVector2D positionSpaceShip = m_space->GetSpaceShip()->GetPosition();

  TSize sizeSpaceShip = m_space->GetSpaceShip()->GetSize();

  Box2D spaceShipBox = Box2D::createBox(
        Point2D(positionSpaceShip.x(), positionSpaceShip.y()),
        Point2D(positionSpaceShip.x() + sizeSpaceShip.first,
                positionSpaceShip.y() + sizeSpaceShip.second));

  std::list<TBulletPtr> & lstBullet = m_space->GetAlienBullets();

  for (auto it = begin(lstBullet); it != end(lstBullet);)
  {
    QVector2D position = (*it)->GetPosition();

    TSize size = (*it)->GetSize();

    Box2D bulletBox = Box2D::createBox(
          Point2D(position.x(), position.y()),
          Point2D(position.x() + size.first,
                  position.y() + size.second));

    // If two boxes are not intersected with each other
    // then return false.
    if (Box2D::checkBoxes(spaceShipBox,bulletBox))
    {
      KillSpaceShip((*it)->GetDamage(),(*it)->GetPosition());
      it = lstBullet.erase(it);
    }
    else
    {
      ++it;
    }
  }
}

void GLWidget::KillSpaceShip(uint damage, QVector2D const position)
{
  uint health = m_space->GetSpaceShip()->GetHealth();
  if ((health-damage) > 0)
  {
    m_space->AddExplosion(std::make_shared<Explosion>(
                           position,
                           Images::Instance().GetImageExplosion(),
                           Settings::Instance().m_explosionParameters.m_sizeBig,
                           Settings::Instance().m_explosionParameters.m_lifetimeBig));
    m_space->GetSpaceShip()->SetHealth(health-damage);
  }
  else
  {
    emit gameOver("You lose!");
  }
}

void GLWidget::CheckHitAlien()
{
  std::list<TBulletPtr> & lstBullet = m_space->GetSpaceShipBullets();
  std::list<TAlienPtr> & lstAlien = m_space->GetAliens();

//  if (lstAlien.empty())
//  {
//    emit gameOver("Congratulations! You win!");
//  }

  for (auto itAlien = begin(lstAlien); itAlien != end(lstAlien);)
  {
    QVector2D positionAlien = (*itAlien)->GetPosition();

    TSize sizeAlien = (*itAlien)->GetSize();

    Box2D alienBox = Box2D::createBox(
          Point2D(positionAlien.x(), positionAlien.y()),
          Point2D(positionAlien.x() + sizeAlien.first,
                  positionAlien.y() + sizeAlien.second));
//    LOG(LogLevel::info) << alienBox;
    bool flag = false;

    for (auto it = begin(lstBullet); it != end(lstBullet);)
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
        uint health = (*itAlien)->GetHealth();
        uint damage = (*it)->GetDamage();
        if ((health-damage) > 0)
        {
          m_space->AddExplosion(std::make_shared<Explosion>(
                                 positionAlien,
                                 Images::Instance().GetImageExplosion(),
                                 Settings::Instance().m_explosionParameters.m_size,
                                 Settings::Instance().m_explosionParameters.m_lifetime));
          (*itAlien)->SetHealth(health-damage);
        }
        else
        {
          flag = true;
        }
        it = lstBullet.erase(it);
      }
      else
      {
        ++it;
      }
    }
    if (flag)
    {
      m_space->AddExplosion(std::make_shared<Explosion>(
                             positionAlien,
                             Images::Instance().GetImageExplosion(),
                             Settings::Instance().m_explosionParameters.m_sizeBig,
                             Settings::Instance().m_explosionParameters.m_lifetimeBig));
      itAlien = lstAlien.erase(itAlien);
    }
    else
    {
      ++itAlien;
    }
  }
}

void GLWidget::ShotAlien()
{
  std::list<TAlienPtr> & lst = m_space->GetAliens();

  for (auto it = begin(lst); it != end(lst); ++it)
  {
    if( abs(m_space->GetSpaceShip()->GetPosition().x() - (*it)->GetPosition().x()) < Globals::Width/2 && Random(0,1) <= 0.5f)
    {
      if((*it)->Shot())
      {
        std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(
              (*it)->GetPosition(),
              Images::Instance().GetImageBulletAlien(),
              Settings::Instance().m_bulletParameters.m_damage,
              Settings::Instance().m_bulletParameters.m_size);

        m_space->AddAlienBullet(bullet);
      }
    }
  }
}

void GLWidget::ExplosionLogic()
{
  std::list<TExplosionPtr> & lst = m_space->GetExplosions();

  for (auto it = begin(lst); it != end(lst);)
  {
    if ((*it)->ReduceLifeTime())
    {
      it = lst.erase(it);
    }
    else
    {
      ++it;
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
  if (e->key() == Qt::Key_Backspace)
  {
    std::list<TAlienPtr> & lstAlien = m_space->GetAliens();

    if (!lstAlien.empty())
    {
      lstAlien.clear();
    }
  }
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
          Settings::Instance().m_bulletParameters.m_damage,
          Settings::Instance().m_bulletParameters.m_size);

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

void GLWidget::SpaceShipBulletsLogic(float const & elapsedSeconds)
{
  // Loop over space ship bullets and delete it if needed.
  std::list<TBulletPtr> & lst = m_space->GetSpaceShipBullets();

  uint rate = m_space->GetSpaceShip()->GetRate();

  for (auto it = begin(lst); it != end(lst);)
  {
    (*it)->IncreaseY(elapsedSeconds * rate);

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

void GLWidget::AlienBulletsLogic(float const & elapsedSeconds)
{
  // Loop over space ship bullets and delete it if needed.
  std::list<TBulletPtr> & lst = m_space->GetAlienBullets();

  for (auto it = begin(lst); it != end(lst);)
  {
    (*it)->DecreaseY(elapsedSeconds * Settings::Instance().m_alienParameters.m_rate);

    if ((*it)->GetPosition().y() == 0.0f)
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

void GLWidget::AlienLogic(float const & elapsedSeconds)
{
  // Loop over aliens.
  std::list<TAlienPtr> & lst = m_space->GetAliens();

  for (auto itAlien : lst)
  {
    if (itAlien->GetSpeed() > 0)
    {
      itAlien->IncreaseX(elapsedSeconds * (itAlien->GetAbsoluteSpeed()));
    }
    else
    {
      itAlien->DecreaseX(elapsedSeconds * (itAlien->GetAbsoluteSpeed()));
    }
  }
}

void GLWidget::CheckHitObstacle()
{
  std::list<TObstaclePtr > & lstObstacles = m_space->GetObstacles();

  std::list<TBulletPtr> & lstBulletsAlien = m_space->GetAlienBullets();

  std::list<TBulletPtr> & lstBulletsSpaceShip = m_space->GetSpaceShipBullets();

  // Loop over obstacles.
  for (auto itObstacle = begin(lstObstacles); itObstacle != end(lstObstacles);)
  {
    QVector2D positionObstacle = (*itObstacle)->GetPosition();

    TSize sizeObstacle = (*itObstacle)->GetSize();

    Box2D obstacleBox = Box2D::createBox(
        Point2D(positionObstacle.x(), positionObstacle.y()),
        Point2D(positionObstacle.x() + sizeObstacle.first,
                positionObstacle.y() + sizeObstacle.second));

    bool flag = false;

    // Loop over alien bullets.
    for (auto it = begin(lstBulletsAlien); it != end(lstBulletsAlien); ++it)
    {
      QVector2D position = (*it)->GetPosition();

      TSize size = (*it)->GetSize();

      Box2D bulletBox = Box2D::createBox(
          Point2D(position.x(), position.y()),
          Point2D(position.x() + size.first,
                  position.y() + size.second));

      // If two boxes are not intersected with each other
      // then return false.
      if (Box2D::checkBoxes(obstacleBox, bulletBox))
      {
        uint health = (*itObstacle)->GetHealth();
        uint damage = (*it)->GetDamage();
        if ((health-damage) > 0)
        {
          m_space->AddExplosion(std::make_shared<Explosion>(
                                 positionObstacle,
                                 Images::Instance().GetImageExplosion(),
                                 Settings::Instance().m_explosionParameters.m_size,
                                 Settings::Instance().m_explosionParameters.m_lifetime));
          (*itObstacle)->SetHealth(health-damage);
        }
        else
        {
           flag = true;
        }
        it = lstBulletsAlien.erase(it);
        break;
      }
    }

    // Loop over space ship bullets if needed.
    if (!flag)
    {
      for (auto it = begin(lstBulletsSpaceShip); it != end(lstBulletsSpaceShip); ++it)
      {
        QVector2D position = (*it)->GetPosition();

        TSize size = (*it)->GetSize();

        Box2D bulletBox = Box2D::createBox(
            Point2D(position.x(), position.y()),
            Point2D(position.x() + size.first,
                    position.y() + size.second));

        // If two boxes are not intersected with each other
        // then return false.
        if (Box2D::checkBoxes(obstacleBox, bulletBox))
        {
          uint health = (*itObstacle)->GetHealth();
          uint damage = (*it)->GetDamage();
          if ((health-damage) > 0)
          {
            m_space->AddExplosion(std::make_shared<Explosion>(
                                   positionObstacle,
                                   Images::Instance().GetImageExplosion(),
                                   Settings::Instance().m_explosionParameters.m_size,
                                   Settings::Instance().m_explosionParameters.m_lifetime));
            (*itObstacle)->SetHealth(health-damage);
          }
          else
          {
             flag = true;
          }
          it = lstBulletsAlien.erase(it);
          break;
        }
      }
    }

    // Make explosion if needed.
    if (flag)
    {
      m_space->AddExplosion(std::make_shared<Explosion>(
          positionObstacle,
          Images::Instance().GetImageExplosion(),
          Settings::Instance().m_explosionParameters.m_sizeBig,
          Settings::Instance().m_explosionParameters.m_lifetimeBig));

      itObstacle = lstObstacles.erase(itObstacle);
    }
    else
    {
      ++itObstacle;
    }
  }
}

void GLWidget::StarLogic()
{
  for (auto it = m_random.begin() ; it != m_random.end(); ++it)
  {
    if((*it).m_periodStar < 1.0)
    {
      (*it).m_periodStar += 0.001f;
    }
    else
    {
      (*it).m_randomStar = std::make_pair(Random(0,1), Random(0,1));
      (*it).m_periodStar = 0.0f;
    }
  }
}

void GLWidget::SetPosition(int w, int h)
{
  QVector2D position = m_space->GetSpaceShip()->GetPosition();
  m_space->GetSpaceShip()->SetPosition(QVector2D(position.x()*w/Globals::Width,position.y()*h/Globals::Height));

  for (auto obstacle : m_space->GetObstacles())
  {
    position = obstacle->GetPosition();
    obstacle->SetPosition(QVector2D(position.x()*w/Globals::Width,position.y()*h/Globals::Height));
  }

  for (auto alien : m_space->GetAliens())
  {
    position = alien->GetPosition();
    alien->SetPosition(QVector2D(position.x()*w/Globals::Width,position.y()*h/Globals::Height));
  }

  for (auto bullet : m_space->GetAlienBullets())
  {
    position = bullet->GetPosition();
    bullet->SetPosition(QVector2D(position.x()*w/Globals::Width,position.y()*h/Globals::Height));
  }

  for (auto bullet : m_space->GetSpaceShipBullets())
  {
    position = bullet->GetPosition();
    bullet->SetPosition(QVector2D(position.x()*w/Globals::Width,position.y()*h/Globals::Height));
  }
}

void GLWidget::CheckSpaceShipCollision()
{
  QVector2D positionSpaceShip = m_space->GetSpaceShip()->GetPosition();

  TSize sizeSpaceShip = m_space->GetSpaceShip()->GetSize();

  Box2D spaceShipBox = Box2D::createBox(
        Point2D(positionSpaceShip.x(), positionSpaceShip.y()),
        Point2D(positionSpaceShip.x() + sizeSpaceShip.first,
                positionSpaceShip.y() + sizeSpaceShip.second));

  std::list<TObstaclePtr > & lstObstacles = m_space->GetObstacles();

  // Loop over obstacles.
  for (auto itObstacle = begin(lstObstacles); itObstacle != end(lstObstacles);)
  {
    QVector2D positionObstacle = (*itObstacle)->GetPosition();

    TSize sizeObstacle = (*itObstacle)->GetSize();

    Box2D obstacleBox = Box2D::createBox(
        Point2D(positionObstacle.x(), positionObstacle.y()),
        Point2D(positionObstacle.x() + sizeObstacle.first,
                positionObstacle.y() + sizeObstacle.second));

    // If two boxes are not intersected with each other
    // then return false.
    if (Box2D::checkBoxes(spaceShipBox, obstacleBox))
    {
      m_space->GetSpaceShip()->SetHealth(0.0f);

      itObstacle = lstObstacles.erase(itObstacle);
    }
    else
    {
      ++itObstacle;
    }
  }

  std::list<TAlienPtr > & lstAliens = m_space->GetAliens();

  // Loop over aliens.
  for (auto itAlien = begin(lstAliens); itAlien != end(lstAliens);)
  {
    QVector2D positionAlien = (*itAlien)->GetPosition();

    TSize sizeAlien = (*itAlien)->GetSize();

    Box2D alienBox = Box2D::createBox(
        Point2D(positionAlien.x(), positionAlien.y()),
        Point2D(positionAlien.x() + sizeAlien.first,
                positionAlien.y() + sizeAlien.second));

    // If two boxes are not intersected with each other
    // then return false.
    if (Box2D::checkBoxes(spaceShipBox, alienBox))
    {
      m_space->GetSpaceShip()->SetHealth(0.0f);

      itAlien = lstAliens.erase(itAlien);
    }
    else
    {
      ++itAlien;
    }
  }
}
