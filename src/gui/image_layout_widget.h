// This widget renders the 2D layout pattern of the hyperspectral image and
// allows the user to add custom edits to the image layout. The spectral
// classes are represented with colors, but in the final image, the 2D layout
// will be converted into a 3D hyperspectral data cube.

#ifndef SRC_GUI_IMAGE_LAYOUT_WIDGET_H_
#define SRC_GUI_IMAGE_LAYOUT_WIDGET_H_

#include <QWidget>

namespace hsi_data_generator {

class ImageLayoutWidget : public QWidget {
 public:
  ImageLayoutWidget();
};

}  // namespace hsi_data_generator

#endif  // SRC_GUI_IMAGE_LAYOUT_WIDGET_H_
