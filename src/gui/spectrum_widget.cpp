#include "gui/spectrum_widget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QtDebug>

#include <vector>

#include "spectrum/spectrum_generator.h"
#include "util/util.h"

namespace hsi_data_generator {
namespace {

static const QString kQtSpectrumStyle = "qt_stylesheets/spectrum_widget.qss";

// Draws the spectrum in render mode, displaying the final spectrum from the
// given spectrum values. It is expected that all spectrum_values are
// normalized between 0 and 1.
void PaintSpectrumRenderMode(
    const double canvas_width,
    const double canvas_height,
    const std::vector<double>& spectrum_values,
    QPainter* painter) {

  const int num_values = spectrum_values.size();
  const double x_stride = canvas_width / static_cast<double>(num_values);
  double previous_x = 0.0;
  double previous_y = canvas_height;
  for (int i = 0; i < num_values; ++i) {
    const double next_x = static_cast<double>(i) * x_stride;
    const double next_y = canvas_height - (canvas_height * spectrum_values[i]);
    painter->drawLine(previous_x, previous_y, next_x, next_y);
    previous_x = next_x;
    previous_y = next_y;
  }
}

// Draws the spectrum in edit mode, which displays the individual peaks such
// that the user can click and drag to edit the spectrum, modifying existing
// peaks or adding new ones.
//
// TODO: Fix the arguments as needed.
void PaintSpectrumEditMode(
    const double canvas_width,
    const double canvas_height,
    QPainter* painter) {

  // TODO: Implement this rendering. For now just draws a line to show
  // something changed.
  painter->drawLine(0, 100, 400, 0);
}

}  // namespace

SpectrumWidget::SpectrumWidget() : display_mode_(SPECTRUM_RENDER_MODE) {
  // Set the stylesheet of this widget.
  const QString stylesheet_string =
      util::GetStylesheetRelativePath(kQtSpectrumStyle);
  setStyleSheet(stylesheet_string);

  // TODO: This is temporary, fix!
  SpectrumGenerator spectrum_generator(200);
  spectrum_values_ = spectrum_generator.GetNormalizedSpectrum();
}

void SpectrumWidget::SetDisplayMode(const SpectrumWidgetDisplayMode mode) {
  display_mode_ = mode;
  update();
}

void SpectrumWidget::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  const double canvas_width = static_cast<double>(width());
  const double canvas_height = static_cast<double>(height());
  if (display_mode_ == SPECTRUM_RENDER_MODE) {
    PaintSpectrumRenderMode(
        canvas_width, canvas_height, spectrum_values_, &painter);
  } else {
    PaintSpectrumEditMode(canvas_width, canvas_height, &painter);
  }
  // TODO: Draw the x-axis step indicator bars (ruler).
  // TODO: Draw the x and y axis numbers.
}

}  // namespace hsi_data_generator
