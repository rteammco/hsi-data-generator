#include "gui/image_layout_widget.h"

#include <QColor>
#include <QPainter>
#include <QPen>
#include <QString>
#include <QtDebug>
#include <QtGlobal>

#include <algorithm>
#include <memory>
#include <vector>

#include "hsi/image_layout.h"
#include "util/util.h"

namespace hsi_data_generator {
namespace {

static const QString kQtImageLayoutStyle =
    "qt_stylesheets/image_layout_widget.qss";

static const QColor kDefaultBackgroundColor = Qt::white;

}  // namespace

ImageLayoutWidget::ImageLayoutWidget(std::shared_ptr<ImageLayout> image_layout)
    : image_layout_(image_layout) {

  setStyleSheet(util::GetStylesheetRelativePath(kQtImageLayoutStyle));

  // By default, there is only a single image class.
  image_class_colors_.resize(1);
  image_class_colors_[0] = kDefaultBackgroundColor;
}

void ImageLayoutWidget::Render(const std::vector<QColor>& class_colors) {
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
      static_cast<double>(image_layout_->GetHeight()) /
      static_cast<double>(canvas_height);
  const double col_scale =
      static_cast<double>(image_layout_->GetWidth()) /
      static_cast<double>(canvas_width);
  const std::vector<int>& image_class_map = image_layout_->GetClassMap();
  for (int y = 0; y < canvas_height; ++y) {
    for (int x = 0; x < canvas_width; ++x) {
      // Back-project to sample the correct color code from the image class map.
      const int row_index = static_cast<int>(row_scale * y);
      const int col_index = static_cast<int>(col_scale * x);
      const int pixel_index = image_layout_->GetMapIndex(col_index, row_index);
      const int class_index = image_class_map.at(pixel_index);
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
