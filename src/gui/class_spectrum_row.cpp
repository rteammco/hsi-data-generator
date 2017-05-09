#include "gui/class_spectrum_row.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QtDebug>

#include "gui/spectrum_widget.h"

namespace hsi_data_generator {
namespace {

// The text displayed on the edit button during its two modes as it gets
// toggled between them.
static const QString kEditButtonEditString = "Edit";
static const QString kEditButtonDoneString = "Done";

}  // namespace

ClassSpectrumRow::ClassSpectrumRow(
    const QString& class_name, const int num_bands) {

  QHBoxLayout* layout = new QHBoxLayout();
  layout->setAlignment(Qt::AlignLeft);

  // layout->addWidget(new QLineEdit(class_name));
  layout->addWidget(new QLabel(class_name));
  // TODO: Color coding label and color picker.

  spectrum_widget_ = new SpectrumWidget(num_bands);
  layout->addWidget(spectrum_widget_);

  spectrum_edit_button_ = new QPushButton(kEditButtonEditString);
  layout->addWidget(spectrum_edit_button_);
  connect(
      spectrum_edit_button_,
      SIGNAL(released()),
      this,
      SLOT(EditButtonPressed()));

  layout->addWidget(new QPushButton("Clone"));

  setLayout(layout);
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

}  // namespace hsi_data_generator
