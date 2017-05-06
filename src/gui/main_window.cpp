#include "gui/main_window.h"

#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QWidget>
#include <QTabWidget>

namespace hsi_data_generator {

MainWindow::MainWindow() {
//  QMenu* file_menu = menuBar()->addMenu(tr("&File"));
//  QToolBar* file_tool_bar = addToolBar(tr("File"));
//
//  QAction* new_action = new QAction(tr("&New"), this);
//  new_action->setShortcuts(QKeySequence::New);
//  new_action->setStatusTip(tr("Create a new file"));
//  connect(new_action, &QAction::triggered, this, &MainWindow::NewFile);
//  file_menu->addAction(new_action);
//  file_tool_bar->addAction(new_action);

  // Set the tabs:
  QWidget* central_widget = new QWidget(this);
  QTabWidget* tabs = new QTabWidget();
  tabs->setParent(this);
//  tabs->setFixedSize(330, 220);
  tabs->addTab(new QWidget(), "Class Spectra");
  tabs->addTab(new QWidget(), "Image Map");
  tabs->addTab(new QWidget(), "Add Noise");
  setCentralWidget(central_widget);
}

}  // namespace hsi_data_generator
