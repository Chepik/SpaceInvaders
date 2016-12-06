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

  std::string level = std::to_string(m_level);

  ReadSettings(level);

  AddAliens(level);

  AddSpaceShip(level);

  AddObstacles(level);

  AddStars();

  m_time.start();
}

void GLWidget::ReadSettings(const std::string & level)
{
  Json::Value settings;
  try
  {
    settings = Util::ReadJson(Globals::SettingsFileName);
  }
  catch(std::exception ex)
  {
    qDebug() << "Can't read settings from a file!";
  }

    m_damageBullet = settings["Level"][std::to_string(m_level)]["BulletDamage"].asUInt();
    m_sizeBullet = std::make_pair(settings["Level"][level]["BulletWidth"].asInt()
                            ,settings["Level"][level]["BulletHeight"].asInt());

    m_lifetimeExplosion = settings["ExplosionLifeTime"].asUInt();
    m_lifetimeExplosionBig = settings["ExplosionLifeTimeBig"].asUInt();
    m_sizeExplosion = std::make_pair(settings["ExplosionWidth"].asInt()
                            ,settings["ExplosionHeight"].asInt());
    m_sizeExplosionBig = std::make_pair(settings["ExplosionWidthBig"].asInt()
                            ,settings["ExplosionHeightBig"].asInt());

}

void GLWidget::AddAliens(const std::string & level)
{
  size_t aliensNumber = 0;
  int speed = 0;
  uint rate = 0;
  uint health = 0;
  TSize size = std::make_pair(0,0);
  size_t aliensRowNumber = 0;

  Json::Value settings;
  try
  {
    settings = Util::ReadJson(Globals::SettingsFileName);
  }
  catch(std::exception ex)
  {
    qDebug() << "Can't read settings from a file!";
  }
  aliensNumber = settings["Level"][level]["AliensNumber"].asUInt();
  speed = settings["Level"][level]["AlienSpeed"].asInt();
  rate = settings["Level"][level]["AlienRate"].asUInt();
  health = settings["Level"][level]["AlienHealth"].asUInt();
  size = std::make_pair(settings["Level"][level]["AlienWidth"].asInt()
                          ,settings["Level"][level]["AlienHeigth"].asInt());
  aliensRowNumber = settings["Level"][level]["AlienRowNumber"].asUInt();

  size_t r = (Globals::Width/aliensNumber);
  int height = settings["Level"][level]["AlienHeigth"].asInt();
  for (size_t j = 0; j < aliensRowNumber; j++)
   for (size_t i = 0; i < aliensNumber; i++)
    {
      m_space->AddAlien(std::make_shared<Alien>(
                          speed,
                          QVector2D(i * r, 600 + j*height),
                          rate,
                          health,
                          Images::Instance().GetImageAlien(),
                          size));
    }
}

void GLWidget::AddSpaceShip(const std::string & level)
{  
  uint health = 0;
  TSize size = std::make_pair(0,0);

  Json::Value settings;
  try
  {
    settings = Util::ReadJson(Globals::SettingsFileName);
  }
  catch(std::exception ex)
  {
    qDebug() << "Can't read settings from a file!";
  }

  health = settings["Level"][level]["SpaceShipHealth"].asUInt();;
  size = std::make_pair(settings["Level"][level]["SpaceShipWidth"].asInt()
                          ,settings["Level"][level]["SpaceShipHeigth"].asInt());

  m_space->SetSpaceShip(std::make_shared<SpaceShip>(
                          QVector2D(Globals::Width/2, size.second),
                          100,
                          health,
                          Images::Instance().GetImageSpaceShip(),
                          size));
}

void GLWidget::AddObstacles(const std::string & level)
{  
  size_t obstaclesNumber = 0;
  uint health = 0;
  TSize size = std::make_pair(0,0);

  Json::Value settings;
  try
  {
    settings = Util::ReadJson(Globals::SettingsFileName);
  }
  catch(std::exception ex)
  {
    qDebug() << "Can't read settings from a file!";
  }
  obstaclesNumber = settings["Level"][level]["ObstacleNumber"].asUInt();
  health = settings["Level"][level]["ObstacleHealth"].asUInt();;
  size = std::make_pair(settings["Level"][level]["ObstacleWidth"].asInt()
                          ,settings["Level"][level]["ObstacleHeigth"].asInt());
  size_t r = (Globals::Width/obstaclesNumber);
  for (size_t i = 0; i < obstaclesNumber; i++)
  {
    m_space->AddObstacle(std::make_shared<Obstacle>(
                           health,
                           QVector2D(i*r, 300),
                           Images::Instance().GetImageObstacle(),
                           size));
  }
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

  ExplosionLogic();

  CheckHitSpaceShip();

  AlienLogic();

  CheckHitAlien();

  ShotAlien();

  SpaceShipBulletsLogic();

  AlienBulletsLogic();

  CheckHitObstacle();

  RenderAlien();

  RenderSpaceShip();

  RenderBullet();

  RenderObstacle();

  RenderExplosion();

  /// Set to zero if it reaches the 1.0 .

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

  RenderStar();
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
  size_t starsNumber = 0;
  TSize size = std::make_pair(0,0);

  Json::Value settings;
  try
  {
    settings = Util::ReadJson(Globals::SettingsFileName);
  }
  catch(std::exception ex)
  {
    qDebug() << "Can't read settings from a file!";
  }
  starsNumber = settings["StarNumber"].asUInt();
  size = std::make_pair(settings["StarWidth"].asInt()
                          ,settings["StarHeigth"].asInt());

  for (size_t i = 1; i <= starsNumber; i++)
  {
    m_space->AddStar(std::make_shared<Star>(
                       QVector2D(200, 600),
                       Images::Instance().GetImageStar(),
                       size));
    RandomStar randomStar;
    randomStar.m_periodStar = Random(0,1);
    randomStar.m_randomStar = std::make_pair(Random(0,1), Random(0,1));
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
  qDebug() << damage;
  if ((health-damage) > 0)
  {
    m_space->AddExplosion(std::make_shared<Explosion>(
                           position,
                           Images::Instance().GetImageExplosion(),
                           m_sizeExplosionBig,
                           m_lifetimeExplosionBig));
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
    LOG(LogLevel::info) << alienBox;
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
                                 m_sizeExplosion,
                                 m_lifetimeExplosion));
          (*itAlien)->SetHealth(health-damage);
          qDebug()<< "SHOT ALIEN";
        }
        else
        {
          flag = true;
        }
        it = lstBullet.erase(it);
        break;
      }
      else
      {
        ++it;
      }
    }
    qDebug()<<"lstBullet.size()="<<lstBullet.size();
    if (flag)
    {
      m_space->AddExplosion(std::make_shared<Explosion>(
                             positionAlien,
                             Images::Instance().GetImageExplosion(),
                             m_sizeExplosionBig,
                             m_lifetimeExplosionBig));
      qDebug() << "KILL ALIEN";
      itAlien = lstAlien.erase(itAlien);
    }
    else
    {
      ++itAlien;
    }
  }
  qDebug() <<"lstAlien.size() = " <<lstAlien.size();
}

void GLWidget::ShotAlien()
{
  std::list<TAlienPtr> & lst = m_space->GetAliens();

  for (auto it = begin(lst); it != end(lst); ++it)
  {
    if( abs(m_space->GetSpaceShip()->GetPosition().x() - (*it)->GetPosition().x()) < Globals::Width/2)
    {
      if((*it)->Shot())
      {
        std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(
              (*it)->GetPosition(),
              Images::Instance().GetImageBulletAlien(),
              m_damageBullet,
              m_sizeBullet);

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
    qDebug() << "ADD BULLET";
    std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(
          m_space->GetSpaceShip()->GetPosition(),
          Images::Instance().GetImageBullet(),
          m_damageBullet,
          m_sizeBullet);

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
    (*it)->DecreaseY(10);

    if ((*it)->GetPosition().y() <= 0.0f)
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

void GLWidget::AlienLogic()
{
  // Loop over aliens.
  std::list<TAlienPtr > & lst = m_space->GetAliens();

  for (auto itAlien : lst)
  {
    if (itAlien->GetSpeed() > 0)
    {
      itAlien->IncreaseX(10.0f);
    }
    else
    {
      itAlien->DecreaseX(10.0f);
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
                                 m_sizeExplosion,
                                 m_lifetimeExplosion));
          (*itObstacle)->SetHealth(health-damage);
          qDebug()<< "SHOT ALIEN";
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
                                   m_sizeExplosion,
                                   m_lifetimeExplosion));
            (*itObstacle)->SetHealth(health-damage);
            qDebug()<< "SHOT ALIEN";
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
          m_sizeExplosionBig,
          m_lifetimeExplosionBig));

      itObstacle = lstObstacles.erase(itObstacle);
    }
    else
    {
      ++itObstacle;
    }
  }
}
