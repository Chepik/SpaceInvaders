#include "game_window.hpp"

#include <QApplication>
#include "gl_widget.hpp"

typedef void (QWidget::*QWidgetVoidSlot)();

GameWindow::GameWindow(QMainWindow *parent)
  : QMainWindow(parent)
{
  m_glWidget = new GLWidget(this, qRgb(20, 20, 50));
  m_timer = new QTimer(this);
  m_settingsButton = new QPushButton("Menu",m_glWidget);
  m_settingsButton->move(15,50);
  m_timer->setInterval(10);
  setCentralWidget(m_glWidget);

  connect(m_timer, &QTimer::timeout, m_glWidget,
          static_cast<QWidgetVoidSlot>(&QWidget::update));

  connect(m_settingsButton, SIGNAL(clicked()),
          this, SLOT(on_menuButtonClicked()));

  connect(this, SIGNAL(moveToMenuPage()),
          parent, SLOT(moveToMenuPage()));

  m_timer->start();
  setFocusPolicy(Qt::StrongFocus);
}

void GameWindow::on_menuButtonClicked()
{
  emit moveToMenuPage();
}
