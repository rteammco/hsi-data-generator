#include "gui/class_spectrum_row.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>

#include <iostream>  // TODO: REMOVE when done testing.

#include "gui/spectrum_widget.h"

namespace hsi_data_generator {

ClassSpectrumRow::ClassSpectrumRow(const QString& class_name) {
  QHBoxLayout* layout = new QHBoxLayout();
  layout->setAlignment(Qt::AlignLeft);

  // layout->addWidget(new QLineEdit(class_name));
  layout->addWidget(new QLabel(class_name));
  // TODO: Color coding label and color picker.

  spectrum_widget_ = new SpectrumWidget();
  layout->addWidget(spectrum_widget_);

  spectrum_edit_button_ = new QPushButton("Edit");
  layout->addWidget(spectrum_edit_button_);
  connect(
      spectrum_edit_button_,
      SIGNAL(released()),
      this,
      SLOT(EditButtonPressed()));

  layout->addWidget(new QPushButton("Clone"));

  setLayout(layout);
}

void ClassSpectrumRow::EditButtonPressed() {
  std::cout << "EDIT BUTTON PRESSED" << std::endl;
}

}  // namespace hsi_data_generator
