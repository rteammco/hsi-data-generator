#include "gui/main_window.h"

//  #include <QAction>
//  #include <QMenuBar>
//  #include <QToolBar>
#include <QString>
#include <QTabWidget>

#include <memory>
#include <vector>

#include "gui/class_spectra_view.h"
#include "gui/class_spectrum_row.h"
#include "gui/image_degradation_view.h"
#include "gui/image_layout_view.h"

namespace hsi_data_generator {
namespace {

static const QString kClassSpectraViewString = "Class Spectra";
static const QString kImageLayoutViewString = "Image Layout";
static const QString kImageDegradationViewString = "Image Degradation";

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

  std::shared_ptr<std::vector<ClassSpectrumRow*>> class_spectrum_rows(
      new std::vector<ClassSpectrumRow*>());
  ClassSpectraView* spectra_view = new ClassSpectraView(class_spectrum_rows);
  tabs->addTab(spectra_view, kClassSpectraViewString);

  ImageLayoutView* image_layout_view = new ImageLayoutView(class_spectrum_rows);
  tabs->addTab(image_layout_view, kImageLayoutViewString);

  tabs->addTab(new ImageDegradationView(), kImageDegradationViewString);

  setCentralWidget(tabs);
}

}  // namespace hsi_data_generator
