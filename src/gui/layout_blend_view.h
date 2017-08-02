// This widget is a GUI for image layout blending, which allows the user to
// define how different clusters of spectra mix together along bordering
// regions of materials.

#ifndef SRC_GUI_LAYOUT_BLEND_VIEW_H_
#define SRC_GUI_LAYOUT_BLEND_VIEW_H_

#include <QWidget>

#include <memory>
#include <vector>

#include "gui/image_layout_widget.h"
#include "hsi/image_layout.h"
#include "hsi/spectrum.h"

namespace hsi_data_generator {

class LayoutBlendView : public QWidget {
 public:
  LayoutBlendView(
    std::shared_ptr<std::vector<std::shared_ptr<Spectrum>>> spectra,
    std::shared_ptr<ImageLayout> image_layout);

 protected:
  void showEvent(QShowEvent* event) override;

 private:
  std::shared_ptr<std::vector<std::shared_ptr<Spectrum>>> spectra_;
  std::shared_ptr<ImageLayout> image_layout_;
  ImageLayoutWidget* image_layout_widget_ = nullptr;
};

}  // namespace hsi_data_generator

#endif  // SRC_GUI_LAYOUT_BLEND_VIEW_H_
