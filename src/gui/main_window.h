#ifndef SRC_GUI_MAIN_WINDOW_H_
#define SRC_GUI_MAIN_WINDOW_H_

#include "QMainWindow.h"

class MainWindow : public QMainWindow {
//  Q_OBJECT

 public:
  MainWindow();

// TODO: Use "private slots:" instead?
 private:
  void NewFile();
};

#endif  // SRC_GUI_MAIN_WINDOW_H_
