#include "gui/export_view.h"

#include <QFileDialog>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>

#include "util/util.h"

namespace hsi_data_generator {
namespace {

static const QString kQtExportViewStyle = "qt_stylesheets/export_view.qss";

static const QString kExportButtonString = "Export HSI";
static const QString kSaveFileDialogName = "Save HSI File";

}  // namespace

ExportView::ExportView() {
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
}

}  // namespace hsi_data_generator
