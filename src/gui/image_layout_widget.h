// This widget renders the 2D layout pattern of the hyperspectral image and
// allows the user to add custom edits to the image layout. The spectral
// classes are represented with colors, but in the final image, the 2D layout
// will be converted into a 3D hyperspectral data cube.

#ifndef SRC_GUI_IMAGE_LAYOUT_WIDGET_H_
#define SRC_GUI_IMAGE_LAYOUT_WIDGET_H_

#include <QColor>
#include <QPaintEvent>
#include <QWidget>

#include <vector>

#include "image/image_layout.h"

namespace hsi_data_generator {

class ImageLayoutWidget : public QWidget {
  Q_OBJECT

 public:
  // Initialize the image with the given width and height. This size can be
  // modified later to reside the image.
  ImageLayoutWidget(const int width, const int height);

  // TODO: Add option to change the image size.

  // Generates an internal representation of a stripe pattern for the given
  // number of spectral classes.
  //
  // This generates a pattern of horizontal stripes, alternating by class, from
  // the top to the bottom of the image.
  void GenerateHorizontalStripesLayout(const int num_classes);

  // This generates a pattern of vertical stripes, alternating by class, from
  // the left to the right of the image.
  void GenerateVerticalStripesLayout(const int num_classes);

  // This generates a grid pattern, where the classes alternate in both the
  // horizontal and vertical directions.
  void GenerateGridLayout(const int num_classes);

  // This generates a random layout, where each pixel is assigned a class label
  // with a uniformly random probability.
  void GenerateRandomLayout(const int num_classes);

  // Renders the image and updates the display, using the given colors for the
  // different spectral classes. The number of colors must match the number of
  // classes used to generate the pattern.
  void Render(const std::vector<QColor>& class_colors);

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  // The internal layout.
  ImageLayout* image_layout_;

  // The size of the image over which the pattern will be generated. This size
  // does not have to match the size of the displayed widget on the screen.
  int image_width_;
  int image_height_;
  // A pixel map which maps indices to a 2D image grid.
  std::vector<int> image_class_map_;

  // A color map that's set when Render() is called and used to render the
  // image layout.
  std::vector<QColor> image_class_colors_;
};

}  // namespace hsi_data_generator

#endif  // SRC_GUI_IMAGE_LAYOUT_WIDGET_H_
