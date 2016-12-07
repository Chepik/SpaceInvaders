#pragma once

#include <QWidget>
#include <QString>

namespace Ui {
class GameOver;
}

class GameOverPage : public QWidget
{
  Q_OBJECT

public:
  explicit GameOverPage(QWidget * parent = nullptr,
                    QString const & message = "");
  ~GameOverPage();

private slots:
  void on_menuButton_clicked();

signals:
  void moveToMenuPage();

private:
  Ui::GameOver * ui;
  QString m_message;
};
