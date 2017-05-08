#include <QApplication>
#include <QFile>
#include <QString>

#include "gui/main_window.h"
#include "util/util.h"

static const QString kQtStyleSheetPath =
    hsi_data_generator::util::GetAbsolutePath("qt_stylesheets/main_style.qss");

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  // Load the stylesheet:
  QFile stylesheet_file(kQtStyleSheetPath);
  stylesheet_file.open(QFile::ReadOnly);
  QString stylesheet_string = QLatin1String(stylesheet_file.readAll());
  app.setStyleSheet(stylesheet_string);

  // Create the main window:
  hsi_data_generator::MainWindow main_window;
  main_window.show();
  return app.exec();
}
