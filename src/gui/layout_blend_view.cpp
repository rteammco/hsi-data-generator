#include "gui/layout_blend_view.h"

#include <QHBoxLayout>
#include <QSizePolicy>

#include <memory>
#include <vector>

#include "gui/image_layout_widget.h"
#include "hsi/image_layout.h"
#include "hsi/spectrum.h"

namespace hsi_data_generator {
namespace {

// Updates the rendered visualization colors in the ImageLayoutWidget. The new
// layout display is then rendered.
void UpdateLayoutVisualization(
    const std::vector<std::shared_ptr<Spectrum>>& spectra,
    ImageLayoutWidget* image_layout_widget) {

  std::vector<QColor> class_colors;
  for (const std::shared_ptr<Spectrum> spectrum : spectra) {
    class_colors.push_back(spectrum->GetColor());
  }
  image_layout_widget->SetClassColors(class_colors);
  image_layout_widget->Render();
}

}  // namespace

LayoutBlendView::LayoutBlendView(
    std::shared_ptr<std::vector<std::shared_ptr<Spectrum>>> spectra,
    std::shared_ptr<ImageLayout> image_layout)
    : spectra_(spectra), image_layout_(image_layout) {

  QHBoxLayout* layout = new QHBoxLayout();
  layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  setLayout(layout);

  image_layout_widget_ = new ImageLayoutWidget(image_layout_);
  image_layout_widget_->setSizePolicy(
      QSizePolicy::Expanding, QSizePolicy::Preferred);
  layout->addWidget(image_layout_widget_);

  // TODO: Whenever it comes into view, set class colors, and do a "render" -
  // but do a FULL render here.
  UpdateLayoutVisualization(*spectra_, image_layout_widget_);
}

void LayoutBlendView::showEvent(QShowEvent* event) {
  UpdateLayoutVisualization(*spectra_, image_layout_widget_);
}

}  // namespace hsi_data_generator
