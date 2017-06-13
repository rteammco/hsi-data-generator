// This widget provides a GUI for designing the image layout, in which all of
// the spectral classes created in the ClassSpectraView will be arranged
// spatially to form an image. This widget allows users to create pre-defined
// or custom patterns in the generated data to produce challenging data sets.

#ifndef SRC_GUI_IMAGE_LAYOUT_VIEW_H_
#define SRC_GUI_IMAGE_LAYOUT_VIEW_H_

#include <QLayout>
#include <QLineEdit>
#include <QPaintEvent>
#include <QShowEvent>
#include <QWidget>

#include <memory>
#include <vector>

#include "gui/image_layout_widget.h"
#include "hsi/image_layout.h"
#include "hsi/spectrum.h"

namespace hsi_data_generator {

class ImageLayoutView : public QWidget {
  Q_OBJECT

 public:
  explicit ImageLayoutView(
      std::shared_ptr<std::vector<std::shared_ptr<Spectrum>>> spectra,
      std::shared_ptr<ImageLayout> image_layout);

  // Call whenever the GUI needs updating after changes to the spectrum or
  // layout that affects the spectra_ list or the image_layout_. This is called
  // automatically when the view comes into focus.
  void UpdateGUI();

 protected:
  void showEvent(QShowEvent* event) override;

 private slots:  // NOLINT
  void HorizontalStripesButtonPressed();
  void VerticalStripesButtonPressed();
  void GridButtonPressed();
  void RandomButtonPressed();
  void SizeInputChanged();

 private:
  // The Spectrum objects generated in the ClassSpectrumView.
  std::shared_ptr<std::vector<std::shared_ptr<Spectrum>>> spectra_;

  // TODO: Some comments.
  std::shared_ptr<ImageLayout> image_layout_;

  ImageLayoutWidget* image_layout_widget_ = nullptr;

  // The input fields for width and height adjustment of the image layout.
  QLineEdit* width_input_ = nullptr;
  QLineEdit* height_input_ = nullptr;

  QLayout* class_names_layout_ = nullptr;
};

}  // namespace hsi_data_generator

#endif  // SRC_GUI_IMAGE_LAYOUT_VIEW_H_
