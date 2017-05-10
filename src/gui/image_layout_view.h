// This widget provides a GUI for designing the image layout, in which all of
// the spectral classes created in the ClassSpectraView will be arranged
// spatially to form an image. This widget allows users to create pre-defined
// or custom patterns in the generated data to produce challenging data sets.

#ifndef SRC_GUI_IMAGE_LAYOUT_VIEW_H_
#define SRC_GUI_IMAGE_LAYOUT_VIEW_H_

#include <QLayout>
#include <QShowEvent>
#include <QWidget>

#include <memory>
#include <vector>

#include "gui/class_spectrum_row.h"
#include "gui/image_layout_widget.h"

namespace hsi_data_generator {

class ImageLayoutView : public QWidget {
  Q_OBJECT

 public:
  ImageLayoutView(
      std::shared_ptr<std::vector<ClassSpectrumRow*>> class_spectrum_rows);

 protected:
  void showEvent(QShowEvent* event) override;

 private:
  // This list of spectra is maintained by ClassSpectraView. We track a local
  // reference to display all of the classes appropriately.
  std::shared_ptr<std::vector<ClassSpectrumRow*>> class_spectrum_rows_;

  ImageLayoutWidget* image_layout_widget_ = nullptr;

  QLayout* class_names_layout_ = nullptr;
};

}  // namespace hsi_data_generator

#endif  // SRC_GUI_IMAGE_LAYOUT_VIEW_H_
