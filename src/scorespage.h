#pragma once

#include <QWidget>

namespace Ui {
class ScoresPage;
}

class ScoresPage : public QWidget
{
 Q_OBJECT

 public:
  explicit ScoresPage(QWidget *parent = 0);
  ~ScoresPage();

 private slots:
  void on_menuButton_clicked();

 signals:
  void moveToMenuPage();

 private:
  Ui::ScoresPage *ui;
};
