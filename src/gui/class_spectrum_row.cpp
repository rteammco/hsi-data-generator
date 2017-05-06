#include "gui/class_spectrum_row.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>

namespace hsi_data_generator {

ClassSpectrumRow::ClassSpectrumRow(const QString& class_name) {
  QHBoxLayout* layout = new QHBoxLayout();
  layout->setAlignment(Qt::AlignLeft);

  // layout->addWidget(new QLineEdit(class_name));
  layout->addWidget(new QLabel(class_name));
  // TODO: Color coding label and color picker.
  layout->addWidget(new QLineEdit("The Spectrum Rendering Goes Here"));
  layout->addWidget(new QPushButton("Edit"));
  layout->addWidget(new QPushButton("Clone"));

  setLayout(layout);
}

}  // namespace hsi_data_generator
