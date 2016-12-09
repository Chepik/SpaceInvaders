#include "mainwindow.hpp"

#include <QDebug>
#include <QComboBox>

#include "menupage.hpp"
#include "game_window.hpp"
#include "settingspage.hpp"
#include "gameoverpage.hpp"
#include "settings.hpp"
#include "except.hpp"
#include "scorespage.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent)
{
  setMinimumSize(Globals::Width, Globals::Height);

  rootPageWidget = new QWidget;

  layoutRootPageWidget = new QVBoxLayout(rootPageWidget);

  pageWidget = new MenuPage(this);

  layoutRootPageWidget->addWidget(pageWidget);

  setCentralWidget(rootPageWidget);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setCurrentIndex(int value)
{
  qDebug() << value;

  if (!layoutRootPageWidget->isEmpty())
  {
    layoutRootPageWidget->removeWidget(pageWidget);

    pageWidget->deleteLater();
    pageWidget = nullptr;
  }

  switch (value) {
    case 0:
      pageWidget = new MenuPage(this);
      break;
    case 1:
      pageWidget = new GameWindow(this, m_currentLevel);
      break;
    case 2:
      pageWidget = new SettingsPage(this);
      break;
    case 3:
      pageWidget = new GameOverPage(this, m_message);
      m_finalScore = 0;
      break;
    case 4:
      pageWidget = new ScoresPage(this);
      break;
    default:
      break;
  }

  if (pageWidget)
  {
    layoutRootPageWidget->addWidget(pageWidget);
  }
}

void MainWindow::moveToGamePage()
{
  qDebug() << "moveToGamePage";

  setCurrentIndex(1);
}

void MainWindow::exitFromGame()
{
  qDebug() << "exitFromGame";

  this->close();
}

void MainWindow::moveToMenuPage()
{
  qDebug() << "moveToMenuPage";

  m_currentLevel = 1;

  setCurrentIndex(0);
}

void MainWindow::moveToSettingsPage()
{
  qDebug() << "moveToSettingsPage";

  setCurrentIndex(2);
}

void MainWindow::moveToNextLevel()
{
  qDebug() << "moveToNextLevel";

  // Increase game level number.
  m_currentLevel++;

  // Load the next game level.
  setCurrentIndex(1);
}

void MainWindow::finishGame(GameState gameState, size_t score)
{
  switch (gameState)
  {
    case GameState::WIN:
    {
      m_finalScore += score;

      // Move to next level.
      if (m_currentLevel < Settings::Instance().m_mainParameters.m_levelsNumber)
      {
        moveToNextLevel();
      }
      else
      {
        m_message = "Game over. Congratulations! You win!\nYour score: " \
            + QString::number(m_finalScore);

        m_currentLevel = 1;

        m_finalScore = 0;

        // Stop the game.
        setCurrentIndex(3);
      }

      return;
    }
    case GameState::LOSE:
    {
      m_message = "Game over. You lose!\nYour score: " \
            + QString::number(m_finalScore);

      m_currentLevel = 1;

      m_finalScore = 0;

      // Stop the game.
      setCurrentIndex(3);

      return;
    }
    case GameState::MENU:
    {
      moveToMenuPage();

      return;
    }
    case GameState::PAUSE:
    {
      return;
    }
  }
}

void MainWindow::moveToScoresPage()
{
  qDebug() << "moveToScoresPage";

  setCurrentIndex(4);
}
