#include "gui/image_layout_view.h"

#include <QColor>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayoutItem>
#include <QString>
#include <QWidget>

#include <vector>

#include "gui/class_spectrum_row.h"

namespace hsi_data_generator {

ImageLayoutView::ImageLayoutView(
    std::shared_ptr<std::vector<ClassSpectrumRow*>> class_spectrum_rows)
    : class_spectrum_rows_(class_spectrum_rows) {

  QHBoxLayout* layout = new QHBoxLayout();
  layout->setAlignment(Qt::AlignTop);
  setLayout(layout);

  // Left-hand column lists the class names and colors.
  // TODO: Maybe this should be a widget instead?
  class_names_layout_ = new QVBoxLayout();
  layout->addLayout(class_names_layout_);

  // Center column is the image display widget.
  // TODO: This should be a stand-alone widget.
  QWidget* image_display = new QWidget();
  image_display->setStyleSheet(
      "min-width: 500px; min-height: 500px; background-color: white");
  layout->addWidget(image_display);

  // Right-hand column is the set of patterns to generate over the image.
  // TODO: Make this interactive, with preview images, etc.
  QVBoxLayout* pattern_list_layout = new QVBoxLayout();
  pattern_list_layout->addWidget(new QLabel("Vertical Stripes"));
  pattern_list_layout->addWidget(new QLabel("Horizontal Stripes"));
  pattern_list_layout->addWidget(new QLabel("Grid"));
  pattern_list_layout->addWidget(new QLabel("Markov"));
  pattern_list_layout->addWidget(new QLabel("Custom Draw"));
  layout->addLayout(pattern_list_layout);
}

void ImageLayoutView::showEvent(QShowEvent* event) {
  const int num_displayed_classes = class_names_layout_->count();
  const int total_num_classes = class_spectrum_rows_->size();
  // TODO: We should do deleting first and then re-inserting instead of
  // replacing, but that's buggy for some reason.
  for (int i = 0; i < total_num_classes; ++i) {
    // Create the class label and set its color.
    const QString class_name = class_spectrum_rows_->at(i)->GetClassName();
    const QColor class_color = class_spectrum_rows_->at(i)->GetClassColor();
    QLabel* new_label = new QLabel(class_name);
    QPalette label_palette;
    label_palette.setColor(new_label->foregroundRole(), class_color);
    new_label->setPalette(label_palette);
    // Update (replace) or add the spectrum classes:
    if (i < num_displayed_classes) {
      QWidget* original_label = class_names_layout_->itemAt(i)->widget();
      class_names_layout_->replaceWidget(original_label, new_label);
      delete original_label;
    } else {
      class_names_layout_->addWidget(new_label);
    }
  }
}

}  // namespace hsi_data_generator
