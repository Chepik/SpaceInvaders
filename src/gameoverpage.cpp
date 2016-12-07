#include "gameoverpage.hpp"
#include "ui_gameoverpage.h"

GameOverPage::GameOverPage(QWidget * parent,
                   QString const & message) :
  QWidget(parent),
  ui(new Ui::GameOver),
  m_message(message)
{
  ui->setupUi(this);

  ui->GameOverMessage->setText(m_message);

  connect(this, SIGNAL(moveToMenuPage()),
          parent, SLOT(moveToMenuPage()));
}

GameOverPage::~GameOverPage()
{
  delete ui;
}

void GameOverPage::on_menuButton_clicked()
{
    emit moveToMenuPage();
}
