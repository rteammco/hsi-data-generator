#include <iostream>

#include "gui/main_window.h"

#include "QApplication"

int main(int argc, char** argv) {
  std::cout << "Hello, world!" << std::endl;
  QApplication app(argc, argv);
  MainWindow main_window;
  main_window.show();
  return app.exec();
}
