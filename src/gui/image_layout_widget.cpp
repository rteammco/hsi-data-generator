#include "gui/image_layout_widget.h"

#include <QColor>
#include <QPainter>
#include <QPen>
#include <QString>
#include <QtDebug>

#include <algorithm>
#include <vector>

#include "util/util.h"

namespace hsi_data_generator {
namespace {

static const QString kQtImageLayoutStyle =
    "qt_stylesheets/image_layout_widget.qss";

// The maximum allowed width (in image pixels) of a single layout stripe.
constexpr int kMaxStripeWidth = 25;

}  // namespace

ImageLayoutWidget::ImageLayoutWidget(const int width, const int height)
    : image_width_(width), image_height_(height) {

  setStyleSheet(util::GetStylesheetRelativePath(kQtImageLayoutStyle));
}

void ImageLayoutWidget::GenerateHorizontalStripesLayout(const int num_classes) {
  const int stripe_width =
      std::min(image_height_ / num_classes, kMaxStripeWidth);
  image_class_map_.clear();
  image_class_map_.resize(image_width_ * image_height_);
  for (int row = 0; row < image_height_; ++row) {
    const int class_index = (row / stripe_width) % num_classes;
    std::fill(
        image_class_map_.begin() + (row * image_width_),
        image_class_map_.begin() + ((row + 1) * image_width_),
        class_index);
  }
}

void ImageLayoutWidget::Render(const std::vector<QColor>& class_colors) {
  const int num_pixels = image_width_ * image_height_;
  if (num_pixels > image_class_map_.size()) {
    qWarning() << "Please generate a layout before attempting to render.";
    return;
  }
  // TODO: This may not be the best solution, by temporarily setting the
  // private class variable to the given color set.
  image_class_colors_ = class_colors;
  update();
}

void ImageLayoutWidget::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, false);
  QPen point_pen;
  point_pen.setCapStyle(Qt::RoundCap);
  point_pen.setWidth(1);
  painter.setPen(point_pen);

  const int canvas_width = width();
  const int canvas_height = height();
  const double row_scale =
      static_cast<double>(image_height_) / static_cast<double>(canvas_height);
  const double col_scale =
      static_cast<double>(image_width_) / static_cast<double>(canvas_width);
  for (int y = 0; y < canvas_height; ++y) {
    for (int x = 0; x < canvas_width; ++x) {
      // Back-project to sample the correct color code from the image class map.
      const int row_index = static_cast<int>(row_scale * y);
      const int col_index = static_cast<int>(col_scale * x);
      const int pixel_index = row_index * image_width_ + col_index;
      const int class_index = image_class_map_.at(pixel_index);
      QColor color;
      if (class_index < image_class_colors_.size()) {
        color = image_class_colors_[class_index];
      } else {
        qWarning() << "Invalid colors: number of colors provided is "
                   << image_class_colors_.size()
                   << " but class index is " << class_index;
        color = Qt::black;
      }
      point_pen.setColor(color);
      painter.setPen(point_pen);
      painter.drawPoint(x, y);
    }
  }
}

}  // namespace hsi_data_generator
