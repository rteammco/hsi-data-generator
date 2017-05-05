#include "gui/main_window.h"

#include <iostream>

#include "QMenuBar.h"
#include "QToolBar.h"

MainWindow::MainWindow() {
  QMenu* file_menu = menuBar()->addMenu(tr("&File"));
  QToolBar* file_tool_bar = addToolBar(tr("File"));

  QAction* new_action = new QAction(tr("&New"), this);
  new_action->setShortcuts(QKeySequence::New);
  new_action->setStatusTip(tr("Create a new file"));
  connect(new_action, &QAction::triggered, this, &MainWindow::NewFile);
  file_menu->addAction(new_action);
  file_tool_bar->addAction(new_action);
}

void MainWindow::NewFile() {
  std::cout << "New File Pressed!" << std::endl;
}
