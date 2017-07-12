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
  // TODO: This code below should work but doesn't when using this inside a
  // QListWidget parent.
//  QPalette palette;
//  palette.setColor(QPalette::Window, color);
//  palette.setBrush(QPalette::Window, color);
//  setAutoFillBackground(true);
//  setPalette(palette);
  // TODO: This is the current hacky workaround.
  const QString color_name = color.name();
  setStyleSheet("background-color: " + color_name + ";");
  color_ = color;
  spectrum_->SetColor(color_);
}

}  // namespace hsi_data_generator
