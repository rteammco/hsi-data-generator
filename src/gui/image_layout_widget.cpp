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
  const int layout_width = image_layout_->GetWidth();
  const int layout_height = image_layout_->GetHeight();
  const std::vector<int>& image_class_map = image_layout_->GetClassMap();
  QImage image(layout_width, layout_height, QImage::Format_RGB32);
  for (int x = 0; x < layout_width; ++x) {
    for (int y = 0; y < layout_height; ++y) {
      const int pixel_index = image_layout_->GetMapIndex(x, y);
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
      image.setPixelColor(x, y, color);
    }
  }
  QPainter painter(this);
  painter.drawImage(0, 0, image.scaled(width(), height()));
}

}  // namespace hsi_data_generator
