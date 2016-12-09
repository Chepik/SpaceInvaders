#include <QApplication>

#pragma once


///
/// It wraps QApplication to catch an exception.
///
class Application : public QApplication
{
public:
  Application(int &argc, char ** argv);

public:
  ///
  /// It catches all exceptions.
  ///
  bool notify(QObject *, QEvent *) override;
};
