#include "gui/class_spectrum_row.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>

#include "gui/spectrum_widget.h"

namespace hsi_data_generator {

ClassSpectrumRow::ClassSpectrumRow(const QString& class_name) {
  QHBoxLayout* layout = new QHBoxLayout();
  layout->setAlignment(Qt::AlignLeft);

  // layout->addWidget(new QLineEdit(class_name));
  layout->addWidget(new QLabel(class_name));
  // TODO: Color coding label and color picker.

  // TODO: Fix up the spectrum widget.
  SpectrumWidget* spectrum_widget = new SpectrumWidget();
  spectrum_widget->setFixedWidth(400);
  spectrum_widget->setFixedHeight(200);
  spectrum_widget->setStyleSheet("background-color: white;");
  layout->addWidget(spectrum_widget);

  layout->addWidget(new QPushButton("Edit"));
  layout->addWidget(new QPushButton("Clone"));

  setLayout(layout);
}

}  // namespace hsi_data_generator
