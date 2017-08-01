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
  layout_visualization_image_ = QImage(width(), height(), QImage::Format_RGB32);
  layout_visualization_image_.fill(kDefaultBackgroundColor);

  // Required for the mouseMoveEvent method to work.
  setMouseTracking(true);
}

void ImageLayoutWidget::Render() {
  const int layout_width = image_layout_->GetWidth();
  const int layout_height = image_layout_->GetHeight();
  const std::vector<int>& image_class_map = image_layout_->GetClassMap();
  layout_visualization_image_ =
      QImage(layout_width, layout_height, QImage::Format_RGB32);
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
      layout_visualization_image_.setPixelColor(x, y, color);
    }
  }
  update();
}

void ImageLayoutWidget::paintEvent(QPaintEvent* event) {
  // Draw the rendered layout image.
  QPainter painter(this);
  const QImage scaled_image =
      layout_visualization_image_.scaled(width(), height());
  painter.drawImage(0, 0, scaled_image);
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
    drag_end_point_ = event->pos();
    is_mouse_dragging_ = false;
    const int widget_width = width();
    const int widget_height = height();
    double component_start_x =
        static_cast<double>(drag_start_point_.x()) /
        static_cast<double>(widget_width);
    double component_end_x =
        static_cast<double>(drag_end_point_.x()) /
        static_cast<double>(widget_width);
    if (component_start_x > component_end_x) {
      const double temp = component_start_x;
      component_start_x = component_end_x;
      component_end_x = temp;
    }
    double component_start_y =
        static_cast<double>(drag_start_point_.y()) /
        static_cast<double>(widget_height);
    double component_end_y =
        static_cast<double>(drag_end_point_.y()) /
        static_cast<double>(widget_height);
    if (component_start_y > component_end_y) {
      const double temp = component_start_y;
      component_start_y = component_end_y;
      component_end_y = temp;
    }
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
    Render();
  }
}

}  // namespace hsi_data_generator
