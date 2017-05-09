#include "gui/class_spectra_widget.h"

#include <QLineEdit>
#include <QtDebug>
#include <QVBoxLayout>

#include "gui/class_spectrum_row.h"

namespace hsi_data_generator {
namespace {

// The default number of bands that is pre-set until the user changes it.
constexpr int kDefaultNumberOfBands = 100;

}  // namespace

ClassSpectraWidget::ClassSpectraWidget() {
  layout_ = new QVBoxLayout();
  layout_->setAlignment(Qt::AlignTop);
  setLayout(layout_);

  number_of_bands_input_ =
      new QLineEdit(QString::number(kDefaultNumberOfBands));
  layout_->addWidget(number_of_bands_input_);
  connect(
      number_of_bands_input_,
      SIGNAL(returnPressed()),
      this,
      SLOT(NumberOfBandsInputChanged()));

  AddClassSpectrumRow("Background", kDefaultNumberOfBands);
  AddClassSpectrumRow("Test Class 1", kDefaultNumberOfBands);
}

void ClassSpectraWidget::NumberOfBandsInputChanged() {
  if (number_of_bands_input_ == nullptr) {
    qCritical() << "Number of bands input is not defined. Cannot change.";
    return;
  }
  const QString num_bands_string = number_of_bands_input_->text();
  const int num_bands = num_bands_string.toInt();
  for (ClassSpectrumRow* row : class_spectrum_rows_) {
    row->SetNumberOfBands(num_bands);
  }
}

void ClassSpectraWidget::AddClassSpectrumRow(
    const QString& name, const int num_bands) {

  ClassSpectrumRow* row = new ClassSpectrumRow(name, num_bands);
  layout_->addWidget(row);
  class_spectrum_rows_.push_back(row);
}

}  // namespace hsi_data_generator
