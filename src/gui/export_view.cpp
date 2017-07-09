#include "gui/export_view.h"

#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>

#include <memory>
#include <vector>

#include "hsi/hsi_exporter.h"
#include "hsi/image_layout.h"
#include "hsi/spectrum.h"
#include "util/util.h"

namespace hsi_data_generator {
namespace {

static const QString kQtExportViewStyle = "qt_stylesheets/export_view.qss";

static const QString kInformationString =
    "Export the HSI data as a binary ENVI image.";
static const QString kExportButtonString = "Export HSI";
static const QString kSaveFileDialogTitle = "Save HSI File";
static const QString kSaveFileErrorDialogTitle = "File Save Error";
static const QString kSaveFileSuccessDialogTitle = "File Saved";
static const QString kSaveFileSuccessDialogMessage =
    "Data was successfully saved to file \"" +
    util::kTextSubPlaceholder + "\".";

}  // namespace

ExportView::ExportView(
    std::shared_ptr<int> num_bands,
    std::shared_ptr<std::vector<std::shared_ptr<Spectrum>>> spectra,
    std::shared_ptr<ImageLayout> image_layout)
    : num_bands_(num_bands), spectra_(spectra), image_layout_(image_layout) {

  setStyleSheet(util::GetStylesheetRelativePath(kQtExportViewStyle));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setAlignment(Qt::AlignTop);
  setLayout(layout);

  QLabel* info_label = new QLabel(kInformationString);
  layout->addWidget(info_label);
  layout->setAlignment(info_label, Qt::AlignCenter);

  QPushButton* export_button = new QPushButton(kExportButtonString);
  layout->addWidget(export_button);
  layout->setAlignment(export_button, Qt::AlignCenter);
  connect(export_button, SIGNAL(released()), this, SLOT(ExportButtonPressed()));
}

void ExportView::ExportButtonPressed() {
  const QString file_name = QFileDialog::getSaveFileName(
      this,
      kSaveFileDialogTitle,          // Dialog save caption.
      util::GetRootCodeDirectory(),  // Default directory.
      "All Files (*)");              // File filter
  if (!file_name.isEmpty()) {
    const HSIDataExporter exporter(spectra_, image_layout_, *num_bands_);
    if (!exporter.SaveFile(file_name)) {
      QMessageBox::critical(
          this,
          kSaveFileErrorDialogTitle,
          exporter.GetErrorMessage());
    } else {
      const QString saved_message =
          util::ReplaceTextSubPlaceholder(kSaveFileSuccessDialogMessage, file_name);
      QMessageBox::information(
          this, kSaveFileSuccessDialogTitle, saved_message);
    }
  }
}

}  // namespace hsi_data_generator
