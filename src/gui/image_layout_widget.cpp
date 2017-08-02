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
#include <QVector>

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
static const QColor kDragRectangleColor1 = Qt::black;
static const QColor kDragRectangleColor2 = Qt::white;
constexpr int kDragDashSpacing = 4;
constexpr int kDragRectangleWidth = 1;

}  // namespace

ImageLayoutWidget::ImageLayoutWidget(std::shared_ptr<ImageLayout> image_layout)
    : image_layout_(image_layout),
      is_mouse_pressed_(false),
      is_mouse_dragging_(false),
      adding_sub_layouts_(false),
      user_selected_class_index_(0) {

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
  // Do nothing if no colors were set (this can also be the case if there are
  // no spectra available).
  if (image_class_colors_.size() == 0) {
    return;
  }
  const int layout_width = image_layout_->GetWidth();
  const int layout_height = image_layout_->GetHeight();
  const std::vector<int>& image_class_map = image_layout_->GetClassMap();
  layout_visualization_image_ =
      QImage(layout_width, layout_height, QImage::Format_RGB32);
  for (int x = 0; x < layout_width; ++x) {
    for (int y = 0; y < layout_height; ++y) {
      const int class_index = image_layout_->GetClassAtPixel(x, y);
      QColor color;
      if (class_index >= 0 && class_index < image_class_colors_.size()) {
        color = image_class_colors_[class_index];
      } else {
        // If the class does not match the number of colors given, or the index
        // is invalid, just set it to the default color.
        color = kDefaultBackgroundColor;
        // TODO: Special color/design for sub-layout index?
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
    const QRect drag_rectangle(mouse_down_point_, mouse_up_point_);
    QPen drag_pen;
    drag_pen.setWidth(kDragRectangleWidth);
    drag_pen.setCapStyle(Qt::RoundCap);
    // Color-alternating dash pattern. First color:
    QPen drag_pen_1 = drag_pen;
    drag_pen_1.setColor(kDragRectangleColor1);
    drag_pen_1.setDashPattern(QVector<qreal>()
        << kDragDashSpacing << kDragDashSpacing);
    drag_pen_1.setStyle(Qt::CustomDashLine);
    painter.setPen(drag_pen_1);
    painter.drawRect(drag_rectangle);
    // Second color (with offset spacing):
    QPen drag_pen_2 = drag_pen;
    drag_pen_2.setColor(kDragRectangleColor2);
    drag_pen_2.setDashPattern(QVector<qreal>()
        << 0 << kDragDashSpacing << kDragDashSpacing << 0);
    drag_pen_2.setStyle(Qt::CustomDashLine);
    painter.setPen(drag_pen_2);
    painter.drawRect(drag_rectangle);
  }
}

void ImageLayoutWidget::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    is_mouse_pressed_ = true;
    mouse_down_point_ = event->pos();
  }
}

void ImageLayoutWidget::mouseMoveEvent(QMouseEvent* event) {
  if (!is_mouse_pressed_) {
    return;
  }
  is_mouse_dragging_ = true;
  mouse_up_point_ = event->pos();
  update();
}

void ImageLayoutWidget::mouseReleaseEvent(QMouseEvent* event) {
  // If mouse was dragged, run the code to fill in a new primitive or
  // sub-layout.
  if (event->button() == Qt::LeftButton && is_mouse_dragging_) {
    mouse_up_point_ = event->pos();
    const int widget_width = width();
    const int widget_height = height();
    double component_start_x =
        static_cast<double>(mouse_down_point_.x()) /
        static_cast<double>(widget_width);
    double component_end_x =
        static_cast<double>(mouse_up_point_.x()) /
        static_cast<double>(widget_width);
    if (component_start_x > component_end_x) {
      const double temp = component_start_x;
      component_start_x = component_end_x;
      component_end_x = temp;
    }
    double component_start_y =
        static_cast<double>(mouse_down_point_.y()) /
        static_cast<double>(widget_height);
    double component_end_y =
        static_cast<double>(mouse_up_point_.y()) /
        static_cast<double>(widget_height);
    if (component_start_y > component_end_y) {
      const double temp = component_start_y;
      component_start_y = component_end_y;
      component_end_y = temp;
    }
    const double component_width = component_end_x - component_start_x;
    const double component_height = component_end_y - component_start_y;
    if (adding_sub_layouts_) {
      image_layout_->AddSubLayout(
          component_start_x,
          component_start_y,
          component_width,
          component_height);
    } else {
      image_layout_->AddLayoutPrimitive(
          component_start_x,
          component_start_y,
          component_width,
          component_height,
          user_selected_class_index_);
    }
    image_layout_->Render();
    Render();
  } else {
    // If mouse was just clicked, check if any sub-layout was moused over, and
    // if so then zoom in to that sub-layout.
    const double click_x =
        static_cast<double>(mouse_down_point_.x()) /
        static_cast<double>(width());
    const double click_y =
        static_cast<double>(mouse_down_point_.y()) /
        static_cast<double>(height());
    if (image_layout_->ZoomInToSubLayout(click_x, click_y)) {
      qInfo() << "Zoomed in";
      // TODO: Re-render.
    } else {
      qInfo() << "Pressed but no zoom";
    }
  }
  is_mouse_pressed_ = false;
  is_mouse_dragging_ = false;
}

}  // namespace hsi_data_generator
