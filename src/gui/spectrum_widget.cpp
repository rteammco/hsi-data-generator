#include "gui/spectrum_widget.h"

#include <QColor>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QPoint>

#include <algorithm>
#include <cmath>
#include <vector>

#include "spectrum/spectrum.h"
#include "util/util.h"

namespace hsi_data_generator {
namespace {

static const QString kQtSpectrumStyle = "qt_stylesheets/spectrum_widget.qss";

// The index assigned to peak_selection_index_ when no peak is selected.
constexpr int kNoPeakSelectedIndex = -1;

// Rendering constants for edit mode:
static const QColor kEditPeakColor = Qt::black;
static const QColor kEditPeakColorSelected(255, 165, 0);  // Orange.
constexpr int kEditPeakPointWidth = 10;
constexpr int kEditPeakLineWidth = 1;
static const QColor kEditRectangleColor(255, 0, 0, 128);  // Transparent red.
constexpr int kEditRectangleLineWidth = 5;

// How far (in pixels) a point needs to be to detect a mouseover.
constexpr int kMouseoverDetectionRadius = 10;

// A simple struct that defines a 2D point on the widget canvas. We have two
// for simplicity: one for absolute (int) coordinates, and one for normalized
// (double) coordinates.
struct AbsolutePoint {
  AbsolutePoint(const int x, const int y) : x(x), y(y) {}
  int x;
  int y;
};
struct NormalizedPoint {
  NormalizedPoint(const double x, const double y) : x(x), y(y) {}
  double x;
  double y;
};

// Returns a normalized (x, y) position (both x and y are between 0 and 1) from
// the given coordinates and canvas size. This function assumes that the canvas
// defines y=0 at the top. This is inverted in the returned value, where y=0 is
// at the bottom (and y=1 is the top).
NormalizedPoint GetNormalizedPosition(
    const int x, const int y, const int width, const int height) {

  const double canvas_width = static_cast<double>(width);
  const double canvas_height = static_cast<double>(height);
  const double x_pos = static_cast<double>(x) / canvas_width;
  const double y_pos = 1.0 - static_cast<double>(y) / canvas_height;
  return NormalizedPoint(x_pos, y_pos);
}

// This takes a position normalized by GetNormalizedPosition() and returns back
// the original absolute pixel position on the canvas (widget screen). The y
// coordinate is again inverted back.
AbsolutePoint GetAbsolutePosition(
    const double x, const double y, const int width, const int height) {

  const double inverted_y = 1.0 - y;
  const int x_pos = static_cast<int>(x * width);
  const int y_pos = static_cast<int>(inverted_y * height);
  return AbsolutePoint(x_pos, y_pos);
}

// Returns true if the given peak is near the given (x, y) mouse coordinate
// over the canvas. "near" is defined as within radius specified by
// kMouseoverDetectionRadius.
bool IsPeakNearCoordinate(
    const PeakDistribution& peak,
    const int mouse_x,
    const int mouse_y,
    const int canvas_width,
    const int canvas_height) {

  const AbsolutePoint peak_position = GetAbsolutePosition(
      peak.position, peak.amplitude, canvas_width, canvas_height);
  const int x_diff = abs(mouse_x - peak_position.x);
  const int y_diff = abs(mouse_y - peak_position.y);
  return (x_diff <= kMouseoverDetectionRadius &&
          y_diff <= kMouseoverDetectionRadius);
}

// Draws the spectrum in render mode, displaying the final spectrum from the
// given spectrum values. It is expected that all spectrum_values are
// normalized between 0 and 1.
void PaintSpectrumRenderMode(
    const int canvas_width,
    const int canvas_height,
    const std::vector<double>& spectrum_values,
    QPainter* painter) {

  const double width = static_cast<double>(canvas_width);
  const double height = static_cast<double>(canvas_height);
  const int num_values = spectrum_values.size();
  const double x_stride = width / static_cast<double>(num_values);
  double previous_x = 0.0;
  double previous_y = height;
  if (num_values > 0) {
    previous_y = height - (height * spectrum_values[0]);
  }
  for (int i = 1; i < num_values; ++i) {
    const double next_x = static_cast<double>(i) * x_stride;
    const double next_y = height - (height * spectrum_values[i]);
    painter->drawLine(previous_x, previous_y, next_x, next_y);
    previous_x = next_x;
    previous_y = next_y;
  }
}

// Draws the spectrum in edit mode, which displays the individual peaks such
// that the user can click and drag to edit the spectrum, modifying existing
// peaks or adding new ones.
void PaintSpectrumEditMode(
    const int canvas_width,
    const int canvas_height,
    const std::vector<PeakDistribution>& peaks,
    const int selected_peak_index,
    QPainter* painter) {

  // The pen used for rendering the regular points:
  QPen point_pen(kEditPeakColor);
  point_pen.setCapStyle(Qt::RoundCap);
  point_pen.setWidth(kEditPeakPointWidth);
  // The pen used for rendering selected (moused over) points:
  QPen point_pen_selected = point_pen;
  point_pen_selected.setColor(kEditPeakColorSelected);
  // The pen used for rendering the lines:
  QPen line_pen(kEditPeakColor);
  line_pen.setWidth(kEditPeakLineWidth);

  const double width = static_cast<double>(canvas_width);
  const double height = static_cast<double>(canvas_height);
  painter->setRenderHint(QPainter::Antialiasing, true);
  for (int i = 0; i < peaks.size(); ++i) {
    const PeakDistribution& peak = peaks.at(i);
    const double peak_x = width * peak.position;
    const double peak_y = height - (height * peak.amplitude);
    // Draw the vertical (height) line:
    painter->setPen(line_pen);
    painter->drawLine(peak_x, peak_y, peak_x, height);
    // Draw the horizontal (width) line:
    const double half_width = (width * peak.width) / 2.0;
    painter->drawLine(peak_x - half_width, peak_y, peak_x + half_width, peak_y);
    // Draw the peak point:
    if (i == selected_peak_index) {
      painter->setPen(point_pen_selected);
    } else {
      painter->setPen(point_pen);
    }
    painter->drawPoint(QPoint(peak_x, peak_y));
  }

  // Outline the whole area to indicate edit mode:
  QPen rectangle_pen(kEditRectangleColor);
  rectangle_pen.setWidth(kEditRectangleLineWidth);
  painter->setPen(rectangle_pen);
  const QRectF canvas_rectangle(0, 0, canvas_width, canvas_height);
  painter->drawRect(canvas_rectangle);
}

}  // namespace

SpectrumWidget::SpectrumWidget(
    const int num_bands, std::shared_ptr<Spectrum> spectrum)
    : display_mode_(SPECTRUM_RENDER_MODE),
      num_bands_(num_bands),
      spectrum_(spectrum),
      peak_selection_index_(kNoPeakSelectedIndex),
      selection_dragging_(false) {

  // Set the stylesheet of this widget.
  setStyleSheet(util::GetStylesheetRelativePath(kQtSpectrumStyle));

  // Required for the mouseMoveEvent method to work.
  setMouseTracking(true);
}

void SpectrumWidget::SetNumberOfBands(const int num_bands) {
  num_bands_ = num_bands;
  update();
}

void SpectrumWidget::Clear() {
  spectrum_->Reset();
  update();
}

void SpectrumWidget::SetDisplayMode(const SpectrumWidgetDisplayMode mode) {
  display_mode_ = mode;
  update();
}

void SpectrumWidget::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  if (display_mode_ == SPECTRUM_RENDER_MODE) {
    const std::vector<double> spectrum_values =
        spectrum_->GenerateSpectrum(num_bands_);
    PaintSpectrumRenderMode(width(), height(), spectrum_values, &painter);
  } else {
    const std::vector<PeakDistribution>& peaks = spectrum_->GetPeaks();
    PaintSpectrumEditMode(
        width(), height(), peaks, peak_selection_index_, &painter);
  }
  // TODO: Draw the x-axis step indicator bars (ruler).
  // TODO: Draw the x and y axis numbers.
}

void SpectrumWidget::mouseMoveEvent(QMouseEvent* event) {
  if (display_mode_ != SPECTRUM_EDIT_MODE) {
    return;
  }
  const int mouse_x = event->x();
  const int mouse_y = event->y();
  const int canvas_width = width();
  const int canvas_height = height();
  // If an already-selected peak is being dragged, update it.
  if (peak_selection_index_ >= 0 && selection_dragging_) {
    NormalizedPoint normalized_pos =
        GetNormalizedPosition(mouse_x, mouse_y, canvas_width, canvas_height);
    const double position = normalized_pos.x;
    const double amplitude = normalized_pos.y;
    const double width = 0.001;  // TODO: Enable setting the width manually.
    spectrum_->UpdatePeak(peak_selection_index_, position, amplitude, width);
    update();
  } else {
    const std::vector<PeakDistribution>& peaks = spectrum_->GetPeaks();
    int new_selection_index = kNoPeakSelectedIndex;
    for (int i = 0; i < peaks.size(); ++i) {
      const bool peak_moused_over = IsPeakNearCoordinate(
          peaks.at(i), mouse_x, mouse_y, canvas_width, canvas_height);
      if (peak_moused_over) {
        new_selection_index = i;
        break;
      }
    }
    if (new_selection_index != peak_selection_index_) {
      peak_selection_index_ = new_selection_index;
      update();
    }
  }
}

void SpectrumWidget::mousePressEvent(QMouseEvent* event) {
  if (display_mode_ != SPECTRUM_EDIT_MODE) {
    return;
  }
  // If a peak was previously selected, flag selection for dragging. Otherwise,
  // add a new peak at the click location.
  const Qt::MouseButton button_pressed = event->button();
  if (peak_selection_index_ >= 0) {
    // Left button = start dragging, right button = delete peak.
    if (button_pressed == Qt::LeftButton) {
      selection_dragging_ = true;
    } else if (button_pressed == Qt::RightButton) {
      spectrum_->DeletePeak(peak_selection_index_);
      peak_selection_index_ = kNoPeakSelectedIndex;
      update();
    }
  } else if (button_pressed == Qt::LeftButton) {
    NormalizedPoint normalized_pos =
        GetNormalizedPosition(event->x(), event->y(), width(), height());
    const double position = normalized_pos.x;
    const double amplitude = normalized_pos.y;
    const double width = 0.001;  // TODO: Enable setting the width manually.
    spectrum_->AddPeak(position, amplitude, width);
    peak_selection_index_ = spectrum_->GetNumPeaks() - 1;
    update();
  }
}

void SpectrumWidget::mouseReleaseEvent(QMouseEvent* event) {
  selection_dragging_ = false;
}

}  // namespace hsi_data_generator
