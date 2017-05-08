#include "gui/spectrum_widget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QPoint>

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
void PaintSpectrumEditMode(
    const double canvas_width,
    const double canvas_height,
    const std::vector<PeakDistribution>& peaks,
    QPainter* painter) {

  // TODO: Use consts for the pen parameters.
  QPen point_pen(Qt::black);
  point_pen.setCapStyle(Qt::RoundCap);
  point_pen.setWidth(10);

  QPen line_pen(Qt::black);
  line_pen.setWidth(1);

  painter->setRenderHint(QPainter::Antialiasing, true);
  for (const PeakDistribution& peak : peaks) {
    const double peak_x = canvas_width * peak.peak_position;
    const double peak_y = canvas_height - (canvas_height * peak.amplitude);
    // Draw the peak point:
    painter->setPen(point_pen);
    painter->drawPoint(QPoint(peak_x, peak_y));
    // Draw the vertical (height) line:
    painter->setPen(line_pen);
    painter->drawLine(peak_x, peak_y, peak_x, canvas_height);
    // Draw the horizontal (width) line:
    const double half_width = (canvas_width * peak.width) / 2.0;
    painter->drawLine(peak_x - half_width, peak_y, peak_x + half_width, peak_y);
  }
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
    PaintSpectrumEditMode(canvas_width, canvas_height, peaks_, &painter);
  }
  // TODO: Draw the x-axis step indicator bars (ruler).
  // TODO: Draw the x and y axis numbers.
}

void SpectrumWidget::mousePressEvent(QMouseEvent* event) {
  if (display_mode_ != SPECTRUM_EDIT_MODE) {
    return;
  }
  const double canvas_width = static_cast<double>(width());
  const double canvas_height = static_cast<double>(height());
  PeakDistribution peak;
  peak.peak_position = static_cast<double>(event->x()) / canvas_width;
  peak.amplitude = 1.0 - static_cast<double>(event->y()) / canvas_height;
  peak.width = 0.1;
  peaks_.push_back(peak);
  update();
}

}  // namespace hsi_data_generator
