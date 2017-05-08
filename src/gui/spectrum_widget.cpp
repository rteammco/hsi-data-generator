#include "gui/spectrum_widget.h"

#include <QPainter>
#include <QPaintEvent>

namespace hsi_data_generator {

SpectrumWidget::SpectrumWidget() {
  // TODO: These should be adjusted using a stylesheet!
  // TODO: Set an "id" or whatever QSS stylesheets use as unique IDs.
  setStyleSheet("background-color: white;");
  setFixedHeight(100);
  setMinimumWidth(400);
}

void SpectrumWidget::paintEvent(QPaintEvent* event) {
  // TODO: Paint the spectrum here.
  QPainter painter(this);
  painter.drawLine(0, 0, 100, 100);
}

}  // namespace hsi_data_generator
