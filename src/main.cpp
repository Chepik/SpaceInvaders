#include <QApplication>
#include <QMainWindow>
#include <QSurfaceFormat>
#include <QDebug>

#include "mainwindow.hpp"
#include "except.hpp"
#include "images.hpp"

int main(int argc, char ** argv)
{
  QApplication a(argc, argv);

  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  //  QSurfaceFormat::setDefaultFormat(format);

  try
  {
    Images::Instance().LoadImages();
  }
  catch (CantLoadImagesException const & ex)
  {
    qDebug() << ex.what();

    return 1;
  }

  MainWindow w;
  w.show();

  return a.exec();
}
