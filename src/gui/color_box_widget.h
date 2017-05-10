// This widget is just a (typically small) displayed box that indicates the
// color of a spectrum class. The color can be adjusted with a color picker
// when the user clicks on this widget.

#ifndef SRC_GUI_COLOR_BOX_WIDGET_H_
#define SRC_GUI_COLOR_BOX_WIDGET_H_

#include <QColor>
#include <QMouseEvent>
#include <QWidget>

namespace hsi_data_generator {

class ColorBoxWidget : public QWidget {
 public:
  // Creates a color box with a random color.
  ColorBoxWidget();

  // Creates a color box with the given color value.
  explicit ColorBoxWidget(const QColor& color);

  // Returns the color.
  QColor GetColor() const {
    return color_;
  }

 protected:
  // If the user clicks on this widget, a color picker will appear, allowing
  // the user to change the color.
  void mousePressEvent(QMouseEvent* event) override;

 private:
  // This is the color that the entire widget will be colored with. Use
  // SetColor to apply new colors and change the color_ value.
  QColor color_;
  void SetColor(const QColor& color);
};

}  // namespace hsi_data_generator

#endif  // SRC_GUI_COLOR_BOX_WIDGET_H_
