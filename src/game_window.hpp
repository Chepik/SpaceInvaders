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
  void gameOver(GameState gameState, size_t score);

signals:
  void moveToMenuPage();
  void moveToNextLevel();
  void finishGame(GameState gameState, size_t score);

private:
  QTimer * m_timer = nullptr;
  QGridLayout * m_layout = nullptr;
  QGLWidget * m_glWidget = nullptr;
};
