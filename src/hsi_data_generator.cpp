#include <QApplication>
#include <QString>

#include "gui/main_window.h"
#include "util/util.h"

static const QString kQtMainStyle = "qt_stylesheets/main_style.qss";

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  // Load the stylesheet:
  const QString stylesheet_string =
      hsi_data_generator::util::GetStylesheetRelativePath(kQtMainStyle);
  app.setStyleSheet(stylesheet_string);

  // Create the main window:
  hsi_data_generator::MainWindow main_window;
  main_window.show();
  return app.exec();
}
