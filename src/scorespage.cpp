#include "scorespage.h"
#include "ui_scorespage.h"

ScoresPage::ScoresPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScoresPage)
{
    ui->setupUi(this);

    connect(this, SIGNAL(moveToMenuPage()),
            parent, SLOT(moveToMenuPage()));
}

ScoresPage::~ScoresPage()
{
    delete ui;
}

void ScoresPage::on_menuButton_clicked()
{
    emit moveToMenuPage();
}
