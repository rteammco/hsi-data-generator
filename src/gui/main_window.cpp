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
#include "gui/export_view.h"
#include "gui/image_layout_view.h"
#include "hsi/image_layout.h"
#include "hsi/spectrum.h"

namespace hsi_data_generator {
namespace {

constexpr int kDefaultImageLayoutWidth = 500;
constexpr int kDefaultImageLayoutHeight = 500;

static const QString kClassSpectraViewString = "Class Spectra";
static const QString kImageLayoutViewString = "Image Layout";
static const QString kExportViewString = "Export";

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

  // The spectral classes shared by all of the views.
  std::shared_ptr<std::vector<std::shared_ptr<Spectrum>>> spectra(
      new std::vector<std::shared_ptr<Spectrum>>());

  std::shared_ptr<ImageLayout> image_layout(
      new ImageLayout(kDefaultImageLayoutWidth, kDefaultImageLayoutHeight));

  ClassSpectraView* spectra_view = new ClassSpectraView(spectra);
  tabs->addTab(spectra_view, kClassSpectraViewString);

  ImageLayoutView* image_layout_view =
      new ImageLayoutView(spectra, image_layout);
  tabs->addTab(image_layout_view, kImageLayoutViewString);

  ExportView* export_view = new ExportView(spectra, image_layout);
  tabs->addTab(export_view, kExportViewString);

  setCentralWidget(tabs);
}

}  // namespace hsi_data_generator
