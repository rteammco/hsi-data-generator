#include "gui/image_layout_view.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

namespace hsi_data_generator {

ImageLayoutView::ImageLayoutView() {
  QHBoxLayout* layout = new QHBoxLayout();
  layout->setAlignment(Qt::AlignTop);
  setLayout(layout);

  // Left-hand column lists the class names and colors.
  // TODO: Generate this using the information from the prior tab.
  QVBoxLayout* class_names_layout = new QVBoxLayout();
  class_names_layout->addWidget(new QLabel("Background"));
  class_names_layout->addWidget(new QLabel("Class A"));
  class_names_layout->addWidget(new QLabel("Class B"));
  class_names_layout->addWidget(new QLabel("Class C"));
  layout->addLayout(class_names_layout);

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

}  // namespace hsi_data_generator
