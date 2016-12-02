#include "mainwindow.hpp"

#include <QDebug>
#include <QComboBox>

#include "menupage.hpp"
#include "game_window.hpp"
#include "settingspage.hpp"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent)
{
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

    delete pageWidget;
    pageWidget = nullptr;
  }

  switch (value) {
    case 0:
      pageWidget = new MenuPage(this);
      break;
    case 1:
      pageWidget = new GameWindow(this);
      break;
    case 2:
      pageWidget = new SettingsPage(this);
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

  setCurrentIndex(0);
}

void MainWindow::moveToSettingsPage()
{
  qDebug() << "moveToSettingsPage";

  setCurrentIndex(2);
}
