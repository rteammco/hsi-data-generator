#include "gui/main_window.h"

//  #include <QAction>
//  #include <QMenuBar>
//  #include <QToolBar>
#include <QString>
#include <QTabWidget>

#include "gui/class_spectra_widget.h"
#include "gui/image_degradation_widget.h"
#include "gui/image_layout_widget.h"

namespace hsi_data_generator {
namespace {

static const QString kClassSpectraWidgetString = "Class Spectra";
static const QString kImageLayoutWidgetString = "Image Layout";
static const QString kImageDegradationWidgetString = "Image Degradation";

}  // namespace

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
  QTabWidget* tabs = new QTabWidget();
  tabs->setParent(this);
  tabs->addTab(new ClassSpectraWidget(), kClassSpectraWidgetString);
  tabs->addTab(new ImageLayoutWidget(), kImageLayoutWidgetString);
  tabs->addTab(new ImageDegradationWidget(), kImageDegradationWidgetString);
  setCentralWidget(tabs);
}

}  // namespace hsi_data_generator
