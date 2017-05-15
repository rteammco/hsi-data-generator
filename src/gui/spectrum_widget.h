// This widget acts as a display canvas for the spectrum itself. It provides
// editing and visualization of a single spectrum.

#ifndef SRC_GUI_SPECTRUM_WIDGET_H_
#define SRC_GUI_SPECTRUM_WIDGET_H_

#include <QEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QWidget>

#include <memory>
#include <vector>

#include "spectrum/spectrum.h"

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
  // Set the default number of bands. The given Spectrum will be edited by this
  // widget through user interaction.
  SpectrumWidget(const int num_bands, std::shared_ptr<Spectrum> spectrum);

  // Adjust the number of bands in the spectrum. This will not change the
  // spectral distribution, since all values are normalized, but it will change
  // the spectral resolution and re-render the current spectrum.
  void SetNumberOfBands(const int num_bands);

  // Clears out all edits, and resets the widget to the default spectrum (every
  // band value is 0).
  void Clear();

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

  void mouseMoveEvent(QMouseEvent* event) override;

  // If the widget is in edit mode, the user can interact with the it to add or
  // modify the peak distribution which are used ot generate the spectrum.
  void mousePressEvent(QMouseEvent* event) override;

 private:
  // The number of bands that will be generated and displayed. All values are
  // normalized, so this can be adjusted at will to fit whatever spectral
  // resolution the user wants.
  int num_bands_;

  // This is the current display mode: render mode displays the spectrum, edit
  // mode allows the user to adjust the spectrum.
  SpectrumWidgetDisplayMode display_mode_;

  // The Spectrum object contains the spectral peaks and can generate the
  // spectrum when it needs to be displayed or returned.
  std::shared_ptr<Spectrum> spectrum_;

  // If a peak is selected during user interaction (mouse move/click/drag
  // events), that peak index will be saved here and tracked for peak editing.
  // peak_selection_index_ should be -1 if no peak is selected.
  int peak_selection_index_;
  QEvent::Type peak_selection_type_;  // TODO: Is this even needed?
};

}  // namespace hsi_data_generator

#endif  // SRC_GUI_SPECTRUM_WIDGET_H_
