#include "gui/class_spectrum_row.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QtDebug>
#include <QVBoxLayout>

#include "gui/spectrum_widget.h"
#include "util/util.h"

namespace hsi_data_generator {
namespace {

static const QString kQtSpectrumRowStyle =
    "qt_stylesheets/class_spectrum_row.qss";

// The text displayed on the edit button during its two modes as it gets
// toggled between them.
static const QString kEditButtonEditString = "Edit";
static const QString kEditButtonDoneString = "Done";
static const QString kClearButtonString = "Clear";
static const QString kCloneButtonString = "Clone";

}  // namespace

ClassSpectrumRow::ClassSpectrumRow(
    const QString& class_name, const int num_bands) {

  setStyleSheet(util::GetStylesheetRelativePath(kQtSpectrumRowStyle));

  QHBoxLayout* layout = new QHBoxLayout();
  layout->setAlignment(Qt::AlignLeft);
  setLayout(layout);

  layout->addWidget(new QLineEdit(class_name));
  // TODO: Color coding label and color picker.

  spectrum_widget_ = new SpectrumWidget(num_bands);
  layout->addWidget(spectrum_widget_);

  // Add the buttons. Stack these vertically at the end of the row.
  QVBoxLayout* button_layout = new QVBoxLayout();

  spectrum_edit_button_ = new QPushButton(kEditButtonEditString);
  button_layout->addWidget(spectrum_edit_button_);
  connect(
      spectrum_edit_button_,
      SIGNAL(released()),
      this,
      SLOT(EditButtonPressed()));

  QPushButton* clear_button = new QPushButton(kClearButtonString);
  button_layout->addWidget(clear_button);
  connect(clear_button, SIGNAL(released()), this, SLOT(ClearButtonPressed()));

  button_layout->addWidget(new QPushButton(kCloneButtonString));

  layout->addLayout(button_layout);
}

void ClassSpectrumRow::SetNumberOfBands(const int num_bands) {
  if (spectrum_widget_ == nullptr) {
    qCritical() << "Spectrum widget not defined. Cannot set number of bands.";
    return;
  }
  spectrum_widget_->SetNumberOfBands(num_bands);
}

void ClassSpectrumRow::EditButtonPressed() {
  // TODO: Better error checks?
  if (spectrum_widget_ == nullptr) {
    qCritical() << "Spectrum widget is not defined. Cannot edit.";
    return;
  }
  if (spectrum_edit_button_ == nullptr) {
    qCritical() << "Edit button is not defined. Cannot edit.";
    return;
  }
  if (spectrum_widget_->GetDisplayMode() == SPECTRUM_RENDER_MODE) {
    spectrum_widget_->SetDisplayMode(SPECTRUM_EDIT_MODE);
    spectrum_edit_button_->setText(kEditButtonDoneString);
  } else {
    spectrum_widget_->SetDisplayMode(SPECTRUM_RENDER_MODE);
    spectrum_edit_button_->setText(kEditButtonEditString);
  }
}

void ClassSpectrumRow::ClearButtonPressed() {
  if (spectrum_widget_ == nullptr) {
    qCritical() << "Spectrum widget is not defined. Cannot clear it.";
    return;
  }
  spectrum_widget_->Clear();
}

}  // namespace hsi_data_generator
