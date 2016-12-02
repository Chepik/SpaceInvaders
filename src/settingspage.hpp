#pragma once

#include <QWidget>

#include <string>

namespace Ui {
class SettingsPage;
}

class SettingsPage : public QWidget
{
  Q_OBJECT

public:
  explicit SettingsPage(QWidget *parent = 0);
  ~SettingsPage();

private slots:
  void on_menuButton_clicked();

  ///
  /// It stores setting data into a file.
  ///
  void on_saveButton_clicked();

signals:
  void moveToMenuPage();

private:
  Ui::SettingsPage *ui;

  std::string settings_file_name = "settings.json";
};
