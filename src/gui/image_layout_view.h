// This widget provides a GUI for designing the image layout, in which all of
// the spectral classes created in the ClassSpectraView will be arranged
// spatially to form an image. This widget allows users to create pre-defined
// or custom patterns in the generated data to produce challenging data sets.

#ifndef SRC_GUI_IMAGE_LAYOUT_VIEW_H_
#define SRC_GUI_IMAGE_LAYOUT_VIEW_H_

#include <QWidget>

namespace hsi_data_generator {

class ImageLayoutView : public QWidget {
  Q_OBJECT

 public:
  ImageLayoutView();
};

}  // namespace hsi_data_generator

#endif  // SRC_GUI_IMAGE_LAYOUT_VIEW_H_
