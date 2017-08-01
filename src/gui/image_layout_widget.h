// This widget renders the 2D layout pattern of the hyperspectral image and
// allows the user to add custom edits to the image layout. The spectral
// classes are represented with colors, but in the final image, the 2D layout
// will be converted into a 3D hyperspectral data cube.

#ifndef SRC_GUI_IMAGE_LAYOUT_WIDGET_H_
#define SRC_GUI_IMAGE_LAYOUT_WIDGET_H_

#include <QColor>
#include <QImage>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPoint>
#include <QWidget>

#include <memory>
#include <vector>

#include "hsi/image_layout.h"

namespace hsi_data_generator {

class ImageLayoutWidget : public QWidget {
  Q_OBJECT

 public:
  // Initialize the image with the given width and height. This size can be
  // modified later to reside the image.
  explicit ImageLayoutWidget(std::shared_ptr<ImageLayout> image_layout);

  // Set the class colors that will be used to render the layout visualization.
  // The given number of colors should match the number of classes in the
  // layout; otherwise a warning will be reported.
  void SetClassColors(const std::vector<QColor>& class_colors) {
    image_class_colors_ = class_colors;
  }

  // Renders the layout image using the given colors (see SetClassColors()).
  // This does not repaint the widget (use update() instead), but forces the
  // layout to render the class map and generates the color representation
  // image.
  void Render();

 protected:
  void paintEvent(QPaintEvent* event) override;

  // If the user presses down, they can start drawing a rectangle to add a new
  // layout component.
  void mousePressEvent(QMouseEvent* event) override;

  // Once dragging, draw in the area for the new component.
  void mouseMoveEvent(QMouseEvent* event) override;

  // Releasing the mouse stops the drawing.
  void mouseReleaseEvent(QMouseEvent* event) override;

 private:
  // The internal layout.
  std::shared_ptr<ImageLayout> image_layout_;

  // A color map that's set when Render() is called and used to render the
  // image layout.
  std::vector<QColor> image_class_colors_;

  // This image is assigned colors to represent the layout when Render() is
  // called. Every time the widget is repainted, this image is drawn.
  QImage layout_visualization_image_;

  // True if the mouse is currently being dragged to draw a new rectange.
  bool is_mouse_dragging_;
  QPoint drag_start_point_;
  QPoint drag_end_point_;
};

}  // namespace hsi_data_generator

#endif  // SRC_GUI_IMAGE_LAYOUT_WIDGET_H_
