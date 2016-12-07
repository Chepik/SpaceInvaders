#include "application.hpp"

#include <QDebug>

Application::Application(int & argc, char ** argv)
  : QApplication(argc, argv) {}

bool Application::notify(QObject * receiver, QEvent * event)
{
  try
  {
    return QApplication::notify(receiver, event);
  }
  catch (std::exception const & ex)
  {
    qDebug() << ex.what();

    this->exit();

    return false;
  }
}
