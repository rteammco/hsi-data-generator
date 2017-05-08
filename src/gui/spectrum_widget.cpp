#include "gui/spectrum_widget.h"

#include <QPainter>
#include <QPaintEvent>

#include "util/util.h"

namespace hsi_data_generator {
namespace {

static const QString kQtSpectrumStyle = "qt_stylesheets/spectrum_widget.qss";

}  // namespace

SpectrumWidget::SpectrumWidget() {
  const QString stylesheet_string =
      util::GetStylesheetRelativePath(kQtSpectrumStyle);
  setStyleSheet(stylesheet_string);
}

void SpectrumWidget::paintEvent(QPaintEvent* event) {
  // TODO: Paint the spectrum here.
  QPainter painter(this);
  painter.drawLine(0, 0, 100, 100);
}

}  // namespace hsi_data_generator
