#include "gui/spectrum_widget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QtDebug>

#include "spectrum/spectrum_generator.h"
#include "util/util.h"

namespace hsi_data_generator {
namespace {

static const QString kQtSpectrumStyle = "qt_stylesheets/spectrum_widget.qss";

}  // namespace

SpectrumWidget::SpectrumWidget() {
  // Set the stylesheet of this widget.
  const QString stylesheet_string =
      util::GetStylesheetRelativePath(kQtSpectrumStyle);
  setStyleSheet(stylesheet_string);

  // TODO: This is temporary, fix!
  SpectrumGenerator spectrum_generator(200);
  spectrum_values_ = spectrum_generator.GetNormalizedSpectrum();
}

void SpectrumWidget::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  const double canvas_width = static_cast<double>(width());
  const double canvas_height = static_cast<double>(height());
  const int num_values = spectrum_values_.size();
  const double x_stride = canvas_width / static_cast<double>(num_values);

  // Draw in the spectrum itself:
  double previous_x = 0.0;
  double previous_y = canvas_height;
  for (int i = 0; i < num_values; ++i) {
    const double next_x = static_cast<double>(i) * x_stride;
    const double next_y = canvas_height - (canvas_height * spectrum_values_[i]);
    painter.drawLine(previous_x, previous_y, next_x, next_y);
    previous_x = next_x;
    previous_y = next_y;
  }

  // TODO: Draw the x-axis step indicator bars (ruler).
  // TODO: Draw the x and y axis numbers.
}

}  // namespace hsi_data_generator
