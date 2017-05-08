// This widget acts as a display canvas for the spectrum itself. It provides
// editing and visualization of a single spectrum.

#ifndef SRC_GUI_SPECTRUM_WIDGET_H_
#define SRC_GUI_SPECTRUM_WIDGET_H_

#include <QPaintEvent>
#include <QWidget>

namespace hsi_data_generator {

class SpectrumWidget : public QWidget {
 protected:
  void paintEvent(QPaintEvent* event) override;
};

}  // namespace hsi_data_generator

#endif  // SRC_GUI_SPECTRUM_WIDGET_H_
