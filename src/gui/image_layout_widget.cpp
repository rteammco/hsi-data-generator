#include "gui/image_layout_widget.h"

#include <QColor>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <QRect>
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

// The animation constants for the user's dragging visualization.
static const QColor kDragRectangleColor = Qt::black;
constexpr int kDragRectangleWidth = 1;

}  // namespace

ImageLayoutWidget::ImageLayoutWidget(std::shared_ptr<ImageLayout> image_layout)
    : image_layout_(image_layout), is_mouse_dragging_(false) {

  setStyleSheet(util::GetStylesheetRelativePath(kQtImageLayoutStyle));

  // By default, there is only a single image class.
  image_class_colors_.resize(1);
  image_class_colors_[0] = kDefaultBackgroundColor;

  // Required for the mouseMoveEvent method to work.
  setMouseTracking(true);
}

void ImageLayoutWidget::Render(const std::vector<QColor>& class_colors) {
  // TODO: This may not be the best solution, by temporarily setting the
  // private class variable to the given color set.
  image_class_colors_ = class_colors;
  update();
}

void ImageLayoutWidget::paintEvent(QPaintEvent* event) {
  // TODO: Paint this into the QImage buffer in Render(). Then just transfer
  // the buffer. This will save a lot of compute power when repainting for the
  // sake of the drag animations.
  const int layout_width = width();
  const int layout_height = height();
  const std::vector<int>& image_class_map = image_layout_->GetClassMap();
  QImage image(layout_width, layout_height, QImage::Format_RGB32);
  for (int x = 0; x < layout_width; ++x) {
    for (int y = 0; y < layout_height; ++y) {
      const int class_index = image_layout_->GetClassAtPixel(x, y);
      QColor color;
      if (class_index < image_class_colors_.size()) {
        color = image_class_colors_[class_index];
      } else {
        qWarning() << "Invalid colors: number of colors provided is "
                   << image_class_colors_.size()
                   << " but class index is " << class_index;
        color = Qt::black;
      }
      image.setPixelColor(x, y, color);
    }
  }
  QPainter painter(this);
  painter.drawImage(0, 0, image.scaled(width(), height()));

  // If the user is dragging to draw a rectangle, draw that in.
  if (is_mouse_dragging_) {
    const QRect drag_rectangle(drag_start_point_, drag_end_point_);
    QPen drag_pen(kDragRectangleColor);
    drag_pen.setWidth(kDragRectangleWidth);
    drag_pen.setCapStyle(Qt::RoundCap);
    painter.setPen(drag_pen);
    painter.drawRect(drag_rectangle);
  }
}

void ImageLayoutWidget::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    is_mouse_dragging_ = true;
    drag_start_point_ = event->pos();
  }
}

void ImageLayoutWidget::mouseMoveEvent(QMouseEvent* event) {
  if (!is_mouse_dragging_) {
    return;
  }
  drag_end_point_ = event->pos();
  update();
}

void ImageLayoutWidget::mouseReleaseEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    is_mouse_dragging_ = false;
    const int layout_width = width();
    const int layout_height = width();
    const double component_start_x =
        static_cast<double>(drag_start_point_.x()) /
        static_cast<double>(layout_width);
    const double component_start_y =
        static_cast<double>(drag_start_point_.y()) /
        static_cast<double>(layout_height);
    const double component_end_x =
        static_cast<double>(drag_end_point_.x()) /
        static_cast<double>(layout_width);
    const double component_end_y =
        static_cast<double>(drag_end_point_.y()) /
        static_cast<double>(layout_height);
    const double component_width = component_end_x - component_start_x;
    const double component_height = component_end_y - component_start_y;
    const int component_class = 0;  // TODO: select the right class!
    image_layout_->AddLayoutPrimitive(
        component_start_x,
        component_start_y,
        component_width,
        component_height,
        component_class);
    image_layout_->Render();  // TODO: Handle this in Render() here.
    update();
  }
}

}  // namespace hsi_data_generator
