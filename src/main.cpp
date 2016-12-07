#include <QApplication>
#include <QMainWindow>
#include <QSurfaceFormat>
#include <QDebug>

#include "mainwindow.hpp"
#include "except.hpp"
#include "images.hpp"
#include "application.hpp"

int main(int argc, char ** argv)
{
  Application a(argc, argv);

  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  //  QSurfaceFormat::setDefaultFormat(format);

  MainWindow w;
  w.show();

  return a.exec();
}
