#include <QApplication>
#include <QString>

#include "gui/main_window.h"
#include "util/util.h"

static const QString kQtMainStyle = "qt_stylesheets/main_style.qss";
static const QString kAppIconLink = "resources/app_icon.png";

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  // Load the stylesheet:
  const QString stylesheet_string =
      hsi_data_generator::util::GetStylesheetRelativePath(kQtMainStyle);
  app.setStyleSheet(stylesheet_string);
  // Set app name and icon:
  QIcon app_icon(hsi_data_generator::util::GetAbsolutePath(kAppIconLink));
  app.setWindowIcon(app_icon);

  // Create the main window:
  hsi_data_generator::MainWindow main_window;
  main_window.show();
  return app.exec();
}
