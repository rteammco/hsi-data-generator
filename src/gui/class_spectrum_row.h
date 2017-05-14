// This widget displays and allows editing of a single class spectrum. This
// spectrum and others is used to generate the synthetic image.

#ifndef SRC_GUI_CLASS_SPECTRUM_ROW_H_
#define SRC_GUI_CLASS_SPECTRUM_ROW_H_

#include <QColor>
#include <QPushButton>
#include <QString>
#include <QWidget>

#include <memory>

#include "gui/color_box_widget.h"
#include "gui/spectrum_widget.h"
#include "spectrum/spectrum.h"

namespace hsi_data_generator {

class ClassSpectrumRow : public QWidget {
  Q_OBJECT

 public:
  ClassSpectrumRow(
      const int num_bands,
      std::shared_ptr<Spectrum> spectrum,
      QWidget* parent_view);

  // Updates the number of bands displayed by the SpectrumWidget.
  void SetNumberOfBands(const int num_bands);

  // Returns a clone of the class Spectrum in this ClassSpectrumRow. This does
  // not modify the original spectrum in any way. It is used for cloning the
  // spectra.
  std::shared_ptr<Spectrum> GetSpectrumCopy() const;

 private slots:  // NOLINT
  void ClassNameFieldChanged(const QString& text);
  void EditButtonPressed();
  void ClearButtonPressed();

 private:
  // The Spectrum itself is passed in and edited by the SpectrumWidget
  // contained in this ClassSpectrumRow widget.
  std::shared_ptr<Spectrum> spectrum_;

  // This is the spectrum widget, which displays the spectrum itself, or allows
  // the user to edit it.
  SpectrumWidget* spectrum_widget_ = nullptr;

  // The "Edit" button (changes to the "Save" button in edit mode) toggles the
  // spectrum widget between editing and rendering mode.
  QPushButton* spectrum_edit_button_ = nullptr;
};

}  // namespace hsi_data_generator

#endif  // SRC_GUI_CLASS_SPECTRUM_ROW_H_
