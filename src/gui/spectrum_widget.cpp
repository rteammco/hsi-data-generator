#include "gui/spectrum_widget.h"

#include <QPainter>
#include <QPaintEvent>

namespace hsi_data_generator {

void SpectrumWidget::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);
  QPainter painter(this);
  painter.drawLine(0, 0, 100, 100);
}

}  // namespace hsi_data_generator
