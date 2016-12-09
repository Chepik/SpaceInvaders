#pragma once

#include <QWidget>

namespace Ui {
class MenuPage;
}

class MenuPage : public QWidget
{
  Q_OBJECT

public:
  explicit MenuPage(QWidget *parent = 0);
  ~MenuPage();

private slots:
  void on_settingsButton_clicked();
  void on_startButton_clicked();
  void on_exitButton_clicked();
  void on_scoresButton_clicked();

signals:
  void moveToGamePage();
  void moveToSettingsPage();
  void exitFromGame();
  void moveToScoresPage();

private:
  Ui::MenuPage *ui;
};
