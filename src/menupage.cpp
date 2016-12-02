#include "menupage.hpp"
#include "ui_menupage.h"

MenuPage::MenuPage(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MenuPage)
{
  ui->setupUi(this);

  connect(this, SIGNAL(moveToSettingsPage()),
          parent, SLOT(moveToSettingsPage()));

  connect(this, SIGNAL(moveToGamePage()),
          parent, SLOT(moveToGamePage()));

  connect(this, SIGNAL(exitFromGame()),
          parent, SLOT(exitFromGame()));
}

MenuPage::~MenuPage()
{
  delete ui;
}

void MenuPage::on_settingsButton_clicked()
{
  emit moveToSettingsPage();
}

void MenuPage::on_startButton_clicked()
{
  emit moveToGamePage();
}

void MenuPage::on_exitButton_clicked()
{
  emit exitFromGame();
}
