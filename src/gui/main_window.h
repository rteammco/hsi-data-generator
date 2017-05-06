// The main window of the application contains all of the other widgets
// (defined in other files) and provides a uniform global menu and tab system
// for the user to switch between widgets and app functions.

#ifndef SRC_GUI_MAIN_WINDOW_H_
#define SRC_GUI_MAIN_WINDOW_H_

#include <QMainWindow>

namespace hsi_data_generator {

class MainWindow : public QMainWindow {
//  Q_OBJECT

 public:
  MainWindow();

// TODO: Use "private slots:" instead?
};

}  // namespace hsi_data_generator

#endif  // SRC_GUI_MAIN_WINDOW_H_
