#include "gui/class_spectra_widget.h"

#include <QVBoxLayout>

#include "gui/class_spectrum_row.h"

namespace hsi_data_generator {

ClassSpectraWidget::ClassSpectraWidget() {
  QVBoxLayout* layout = new QVBoxLayout();
  layout->setAlignment(Qt::AlignTop);

  layout->addWidget(new ClassSpectrumRow("Background"));
  layout->addWidget(new ClassSpectrumRow("Test Class 1"));

  setLayout(layout);
}

}  // namespace hsi_data_generator
