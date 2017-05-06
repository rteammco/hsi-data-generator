// This widget provides a GUI for creating and manipulating new spectral
// signatures. These spectra serve as the core spectral classes to construct
// the synthesized HSI data.
//
// As a very simple example, for an RGB image, each class would be defined as a
// unique color, and the image would be constructed by combining these colors
// in a particular spatial layout across the image.

#ifndef SRC_GUI_CLASS_SPECTRA_WIDGET_H_
#define SRC_GUI_CLASS_SPECTRA_WIDGET_H_

#include <QWidget>

namespace hsi_data_generator {

class ClassSpectraWidget : public QWidget {
 public:
  ClassSpectraWidget();
};

}  // namespace hsi_data_generator

#endif  // SRC_GUI_CLASS_SPECTRA_WIDGET_H_
