// This widget provides a GUI for creating and manipulating new spectral
// signatures. These spectra serve as the core spectral classes to construct
// the synthesized HSI data.
//
// As a very simple example, for an RGB image, each class would be defined as a
// unique color, and the image would be constructed by combining these colors
// in a particular spatial layout across the image.

#ifndef SRC_GUI_CLASS_SPECTRA_VIEW_H_
#define SRC_GUI_CLASS_SPECTRA_VIEW_H_

#include <QBoxLayout>
#include <QLineEdit>
#include <QString>
#include <QWidget>

#include <memory>
#include <vector>

#include "gui/class_spectrum_row.h"
#include "spectrum/spectrum.h"

namespace hsi_data_generator {

class ClassSpectraView : public QWidget {
  Q_OBJECT

 public:
  explicit ClassSpectraView(std::shared_ptr<std::vector<Spectrum*>> spectra);

 private slots:  // NOLINT
  void NumberOfBandsInputChanged();
  void NewSpectrumButtonPressed();

 private:
  // The layout used by this widget.
  QBoxLayout* layout_;

  // The input field where the user can set the number of bands.
  QLineEdit* number_of_bands_input_;

  // The current number of bands. This will be the number of bands set to all
  // newly added spectra, and can be updated by the user using the
  // number_of_bands_input_ filed.
  int num_bands_;

  // This number just keeps track of the newly added spectra, and increments
  // every time a new spectrum is added. This ensures unique names of each
  // spectrum.
  int next_spectrum_number_;

  // The actual spectra are passed down and shared between the different view.
  // This view allows for editing the spectra directly.
  std::shared_ptr<std::vector<Spectrum*>> spectra_;

  // A list of all added ClassSpectrumRow widgets. These are referenced to
  // modify the number of bands or to get the spectra.
  std::vector<ClassSpectrumRow*> class_spectrum_rows_;

  // Adds a new ClassSpectrumRow to the widget. It will be displayed and
  // tracked in the class_spectrum_rows_ list.
  void AddClassSpectrumRow(const QString& name, const int num_bands);
};

}  // namespace hsi_data_generator

#endif  // SRC_GUI_CLASS_SPECTRA_VIEW_H_
