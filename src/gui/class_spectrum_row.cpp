#include "gui/class_spectrum_row.h"

#include <QColor>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QtDebug>
#include <QVBoxLayout>
#include <QWidget>

#include <memory>

#include "gui/color_box_widget.h"
#include "gui/spectrum_widget.h"
#include "spectrum/spectrum.h"
#include "util/util.h"

namespace hsi_data_generator {
namespace {

// Stylesheet file path and object reference names.
static const QString kQtSpectrumRowViewStyle =
    "qt_stylesheets/class_spectrum_row.qss";
static const QString kQtClassColorBoxName = "class_color_box";

// The text displayed on the edit button during its two modes as it gets
// toggled between them.
static const QString kEditButtonEditString = "Edit";
static const QString kEditButtonDoneString = "Done";
static const QString kClearButtonString = "Clear";
static const QString kCloneButtonString = "Clone";

}  // namespace

ClassSpectrumRow::ClassSpectrumRow(
    const int num_bands, std::shared_ptr<Spectrum> spectrum)
    : spectrum_(spectrum) {

  setStyleSheet(util::GetStylesheetRelativePath(kQtSpectrumRowViewStyle));

  QHBoxLayout* layout = new QHBoxLayout();
  layout->setAlignment(Qt::AlignLeft);
  setLayout(layout);

  QLineEdit* class_name_field = new QLineEdit(spectrum_->GetName());
  layout->addWidget(class_name_field);
  connect(
      class_name_field,
      SIGNAL(textChanged(const QString&)),
      this,
      SLOT(ClassNameFieldChanged(const QString&)));

  ColorBoxWidget* class_color_box = new ColorBoxWidget(spectrum_);
  class_color_box->setObjectName(kQtClassColorBoxName);  // Name for stylesheet.
  layout->addWidget(class_color_box);

  spectrum_widget_ = new SpectrumWidget(num_bands, spectrum_);
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

  QPushButton* clone_button = new QPushButton(kCloneButtonString);
  clone_button->setEnabled(false);  // TODO: Implement.
  button_layout->addWidget(clone_button);

  layout->addLayout(button_layout);
}

void ClassSpectrumRow::SetNumberOfBands(const int num_bands) {
  if (spectrum_widget_ == nullptr) {
    qCritical() << "Spectrum widget not defined. Cannot set number of bands.";
    return;
  }
  spectrum_widget_->SetNumberOfBands(num_bands);
}

void ClassSpectrumRow::ClassNameFieldChanged(const QString& class_name) {
  if (!class_name.isEmpty()) {
    spectrum_->SetName(class_name);
  }
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
