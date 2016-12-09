#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QGridLayout>
#include <QtOpenGL/QGLWidget>
#include <QPushButton>
#include "game_state.hpp"

class GameWindow : public QMainWindow
{
  Q_OBJECT

public:
  GameWindow(QMainWindow *parent, size_t const & level);

private slots:
  void on_menuButtonClicked();
  void on_nextLevelButtonClicked();
  void gameOver(GameState gameState);

signals:
  void moveToMenuPage();
  void moveToNextLevel();
  void finishGame(GameState gameState);

private:
  QTimer * m_timer = nullptr;
  QGridLayout * m_layout = nullptr;
  QGLWidget * m_glWidget = nullptr;
  QPushButton * m_settingsButton = nullptr;
  QPushButton * m_nextLevelButton = nullptr;
};
