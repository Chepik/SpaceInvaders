#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

public slots:
  void setCurrentIndex(int value);
  void moveToSettingsPage();
  void moveToGamePage();
  void exitFromGame();
  void moveToMenuPage();
  void moveToNextLevel();

public:
  QWidget * rootPageWidget = nullptr;
  QWidget * pageWidget = nullptr;
  QVBoxLayout * layoutRootPageWidget = nullptr;

  // It stores current game level.
  size_t m_currentLevel = 1;
};
