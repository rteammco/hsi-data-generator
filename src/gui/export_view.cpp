#include "gui/export_view.h"

#include <QFileDialog>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>

#include <memory>
#include <vector>

#include "util/util.h"

namespace hsi_data_generator {
namespace {

static const QString kQtExportViewStyle = "qt_stylesheets/export_view.qss";

static const QString kExportButtonString = "Export HSI";
static const QString kSaveFileDialogName = "Save HSI File";

}  // namespace

ExportView::ExportView(
    std::shared_ptr<std::vector<std::shared_ptr<Spectrum>>> spectra,
    std::shared_ptr<ImageLayoutWidget> image_layout_widget)
    : spectra_(spectra), image_layout_widget_(image_layout_widget) {

  setStyleSheet(util::GetStylesheetRelativePath(kQtExportViewStyle));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setAlignment(Qt::AlignTop);
  setLayout(layout);

  QPushButton* export_button = new QPushButton(kExportButtonString);
  layout->addWidget(export_button);
  layout->setAlignment(export_button, Qt::AlignCenter);
  connect(export_button, SIGNAL(released()), this, SLOT(ExportButtonPressed()));
}

void ExportView::ExportButtonPressed() {
  const QString file_name = QFileDialog::getSaveFileName(
      this,
      kSaveFileDialogName,           // Dialog save caption.
      util::GetRootCodeDirectory(),  // Default directory.
      "All Files (*)");              // File filter

  // TODO: Use the spectra_ and image_layout_widget_ variables to generate and
  // save an HSI file at the user-specified save location.
}

}  // namespace hsi_data_generator
