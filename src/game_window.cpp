#include "game_window.hpp"

#include <QApplication>
#include <QtCore/QtCore>
#include "gl_widget.hpp"

typedef void (QWidget::*QWidgetVoidSlot)();

GameWindow::GameWindow(QMainWindow *parent, size_t const & level)
  : QMainWindow(parent)
{
  // It loads the game and initialise it accordingly to the game level number.
  m_glWidget = new GLWidget(this, qRgb(20, 20, 50), level);

  m_timer = new QTimer(this);
  m_timer->setInterval(10);

  m_settingsButton = new QPushButton("Menu", m_glWidget);
  m_settingsButton->move(15, 100);

  m_nextLevelButton = new QPushButton("Next level", m_glWidget);
  m_nextLevelButton->move(15, 130);

  setCentralWidget(m_glWidget);

  connect(m_timer, &QTimer::timeout, m_glWidget,
          static_cast<QWidgetVoidSlot>(&QWidget::update));

  connect(m_settingsButton, SIGNAL(clicked()),
          this, SLOT(on_menuButtonClicked()));

  connect(m_nextLevelButton, SIGNAL(clicked()),
          this, SLOT(on_nextLevelButtonClicked()));

  connect(this, SIGNAL(moveToMenuPage()),
          parent, SLOT(moveToMenuPage()));

  connect(this, SIGNAL(moveToNextLevel()),
          parent, SLOT(moveToNextLevel()));

  connect(this, SIGNAL(finishGame(GameState, size_t)),
          parent, SLOT(finishGame(GameState, size_t)));

  m_timer->start();
}

void GameWindow::on_menuButtonClicked()
{
  emit moveToMenuPage();
}

void GameWindow::on_nextLevelButtonClicked()
{
  emit moveToNextLevel();
}

void GameWindow::gameOver(GameState gameState, size_t score)
{
  m_glWidget->deleteLater();

  emit finishGame(gameState, score);
}
