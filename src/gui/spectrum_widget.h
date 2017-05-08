// This widget acts as a display canvas for the spectrum itself. It provides
// editing and visualization of a single spectrum.

#ifndef SRC_GUI_SPECTRUM_WIDGET_H_
#define SRC_GUI_SPECTRUM_WIDGET_H_

#include <QMouseEvent>
#include <QPaintEvent>
#include <QWidget>

#include <vector>

#include "spectrum/spectrum_generator.h"

namespace hsi_data_generator {

// The display modes for the SpectrumWidget. This is used by the class to
// determine how the information is displayed for the user (just rendering the
// spectrum, or providing an interface for the user to edit it).
enum SpectrumWidgetDisplayMode {
  SPECTRUM_RENDER_MODE,
  SPECTRUM_EDIT_MODE
};

class SpectrumWidget : public QWidget {
 public:
  SpectrumWidget();

  // Switches the rendering and user interaction between render mode (just
  // displaying the spectrum as is) and edit mode (allowing the user to create
  // or modify the spectrum).
  void SetDisplayMode(const SpectrumWidgetDisplayMode mode);

  // Returns the current display mode (edit or render).
  SpectrumWidgetDisplayMode GetDisplayMode() const {
    return display_mode_;
  }

 protected:
  // Uses a QPainter to draw the spectrum, or the current edit visualization,
  // inside the widget.
  void paintEvent(QPaintEvent* event) override;

  void mousePressEvent(QMouseEvent* event) override;

 private:
  // The spectrum values that will be displayed when the widget is in render
  // mode.
  std::vector<double> spectrum_values_;

  // The edit peak locations.
  std::vector<PeakDistribution> peaks_;

  // This is the current.
  SpectrumWidgetDisplayMode display_mode_;
};

}  // namespace hsi_data_generator

#endif  // SRC_GUI_SPECTRUM_WIDGET_H_
