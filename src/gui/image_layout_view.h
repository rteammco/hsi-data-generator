// This widget provides a GUI for designing the image layout, in which all of
// the spectral classes created in the ClassSpectraView will be arranged
// spatially to form an image. This widget allows users to create pre-defined
// or custom patterns in the generated data to produce challenging data sets.

#ifndef SRC_GUI_IMAGE_LAYOUT_VIEW_H_
#define SRC_GUI_IMAGE_LAYOUT_VIEW_H_

#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
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
  void ImportImageButtonPressed();
  void ClearButtonPressed();
  void SizeInputChanged();
  void ClassLabelSelected(QListWidgetItem* selected_item);

 private:
  // The Spectrum objects generated in the ClassSpectrumView.
  std::shared_ptr<std::vector<std::shared_ptr<Spectrum>>> spectra_;

  // A pointer to the ImageLayout which contains all of the underlying layout
  // representation required to display it.
  std::shared_ptr<ImageLayout> image_layout_;

  // The ImageLayoutWidget actually handles displaying a visualization of the
  // layout.
  ImageLayoutWidget* image_layout_widget_ = nullptr;

  // The input fields for width and height adjustment of the image layout.
  QLineEdit* width_input_ = nullptr;
  QLineEdit* height_input_ = nullptr;

  // A list of the spectral class names, color-coded, and user-selectable when
  // editing the layout.
  QListWidget* class_names_list_ = nullptr;
};

}  // namespace hsi_data_generator

#endif  // SRC_GUI_IMAGE_LAYOUT_VIEW_H_
