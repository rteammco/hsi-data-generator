#include "gui/class_spectra_widget.h"

#include <QLineEdit>
#include <QVBoxLayout>

#include "gui/class_spectrum_row.h"

namespace hsi_data_generator {

ClassSpectraWidget::ClassSpectraWidget() {
  QVBoxLayout* layout = new QVBoxLayout();
  layout->setAlignment(Qt::AlignTop);

  const int num_bands = 200;  // TODO: Do not hard-code.
  layout->addWidget(new QLineEdit("200"));  // TODO: the number of spectra.
  layout->addWidget(new ClassSpectrumRow("Background", num_bands));
  layout->addWidget(new ClassSpectrumRow("Test Class 1", num_bands));

  setLayout(layout);
}

}  // namespace hsi_data_generator
