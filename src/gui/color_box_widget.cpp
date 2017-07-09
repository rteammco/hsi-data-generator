#include "gui/color_box_widget.h"

#include <QColor>
#include <QColorDialog>
#include <QMouseEvent>
#include <QPalette>
#include <QtGlobal>

#include <memory>

#include "hsi/spectrum.h"

namespace hsi_data_generator {
namespace {

}  // namespace

ColorBoxWidget::ColorBoxWidget(std::shared_ptr<Spectrum> spectrum)
    : spectrum_(spectrum) {

  SetColor(spectrum_->GetColor());
}

ColorBoxWidget::ColorBoxWidget(
    const QColor& color, std::shared_ptr<Spectrum> spectrum)
    : spectrum_(spectrum) {

  SetColor(color);
}

void ColorBoxWidget::mousePressEvent(QMouseEvent* event) {
  const QColor selected_color = QColorDialog::getColor(color_, this);
  if (selected_color.isValid()) {
    SetColor(selected_color);
  }
}

void ColorBoxWidget::SetColor(const QColor& color) {
  QPalette palette;
  palette.setColor(QPalette::Background, color);
  setAutoFillBackground(true);
  setPalette(palette);
  color_ = color;
  spectrum_->SetColor(color_);
}

}  // namespace hsi_data_generator
