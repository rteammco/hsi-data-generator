#include <string>

#include "gui/main_window.h"

#include "QApplication.h"
#include "QFile.h"
#include "QString.h"

static const std::string kQtStyleSheetPath = "../src/gui/qt_style.qss";

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  // Load the stylesheet:
  QFile stylesheet_file(kQtStyleSheetPath.c_str());
  stylesheet_file.open(QFile::ReadOnly);
  QString stylesheet_string = QLatin1String(stylesheet_file.readAll());
  app.setStyleSheet(stylesheet_string);

  // Create the main window:
  MainWindow main_window;
  main_window.show();
  return app.exec();
}
