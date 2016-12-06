#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QGridLayout>
#include <QtOpenGL/QGLWidget>
#include <QPushButton>

class GameWindow : public QMainWindow
{
  Q_OBJECT

public:
  GameWindow(QMainWindow *parent, size_t const & level);

private slots:
  void on_menuButtonClicked();
  void on_nextLevelButtonClicked();

signals:
  void moveToMenuPage();
  void moveToNextLevel();

private:
  QTimer * m_timer = nullptr;
  QGridLayout * m_layout = nullptr;
  QGLWidget * m_glWidget = nullptr;
  QPushButton * m_settingsButton = nullptr;
  QPushButton * m_nextLevelButton = nullptr;
};
