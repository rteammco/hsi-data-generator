// This widget is just a (typically small) displayed box that indicates the
// color of a spectrum class. The color can be adjusted with a color picker
// when the user clicks on this widget. When the color is updated, the
// associated Spectrum's color will also be modified automatically.

#ifndef SRC_GUI_COLOR_BOX_WIDGET_H_
#define SRC_GUI_COLOR_BOX_WIDGET_H_

#include <QColor>
#include <QMouseEvent>
#include <QWidget>

#include <memory>

#include "hsi/spectrum.h"

namespace hsi_data_generator {

class ColorBoxWidget : public QWidget {
 public:
  // Creates a color box with a random color. The given Spectrum will
  // automatically be re-colored to the random value.
  explicit ColorBoxWidget(std::shared_ptr<Spectrum> spectrum);

  // Creates a color box with the given color value. The given Spectrum will
  // automatically be updated to this color as well.
  ColorBoxWidget(const QColor& color, std::shared_ptr<Spectrum> spectrum);

 protected:
  // If the user clicks on this widget, a color picker will appear, allowing
  // the user to change the color.
  void mousePressEvent(QMouseEvent* event) override;

 private:
  // The Spectrum associated with this color box. Changing the color values
  // will also modify the Spectrum's representative color.
  std::shared_ptr<Spectrum> spectrum_;

  // This is the color that the entire widget will be colored with. Use
  // SetColor to apply new colors and change the color_ value.
  QColor color_;
  void SetColor(const QColor& color);
};

}  // namespace hsi_data_generator

#endif  // SRC_GUI_COLOR_BOX_WIDGET_H_
