// This widget acts as a display canvas for the spectrum itself. It provides
// editing and visualization of a single spectrum.

#ifndef SRC_GUI_SPECTRUM_WIDGET_H_
#define SRC_GUI_SPECTRUM_WIDGET_H_

#include <QPaintEvent>
#include <QWidget>

#include <vector>

namespace hsi_data_generator {

enum SpectrumWidgetPaintMode {
  SPECTRUM_RENDER_MODE,
  SPECTRUM_EDIT_MODE
};

class SpectrumWidget : public QWidget {
 public:
  SpectrumWidget();

  // Switches the rendering and user interaction between render mode (just
  // displaying the spectrum as is) and edit mode (allowing the user to create
  // or modify the spectrum).
  void SetMode(const SpectrumWidgetPaintMode mode);

 protected:
  // Uses a QPainter to draw the spectrum, or the current edit visualization,
  // inside the widget.
  void paintEvent(QPaintEvent* event) override;

 private:
  std::vector<double> spectrum_values_;

  // This is the current.
  SpectrumWidgetPaintMode paint_mode_;
};

}  // namespace hsi_data_generator

#endif  // SRC_GUI_SPECTRUM_WIDGET_H_
