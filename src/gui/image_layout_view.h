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

#include "gui/image_layout_widget.h"
#include "spectrum/spectrum.h"

namespace hsi_data_generator {

class ImageLayoutView : public QWidget {
  Q_OBJECT

 public:
  explicit ImageLayoutView(
      std::shared_ptr<std::vector<std::shared_ptr<Spectrum>>> spectra);

 protected:
  void showEvent(QShowEvent* event) override;

 private slots:  // NOLINT
  void HorizontalStripesButtonPressed();
  void VerticalStripesButtonPressed();
  void GridButtonPressed();
  void RandomButtonPressed();

 private:
  // The Spectrum objects generated in the ClassSpectrumView.
  std::shared_ptr<std::vector<std::shared_ptr<Spectrum>>> spectra_;

  ImageLayoutWidget* image_layout_widget_ = nullptr;

  QLayout* class_names_layout_ = nullptr;
};

}  // namespace hsi_data_generator

#endif  // SRC_GUI_IMAGE_LAYOUT_VIEW_H_
