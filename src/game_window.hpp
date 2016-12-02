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
  GameWindow(QMainWindow *parent = 0);

private slots:
  void on_menuButtonClicked();

signals:
  void moveToMenuPage();

private:
  QTimer * m_timer = nullptr;
  QGridLayout * m_layout = nullptr;
  QGLWidget * m_glWidget = nullptr;
  QPushButton * m_settingsButton = nullptr;
};
