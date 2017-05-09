// This widget displays and allows editing of a single class spectrum. This
// spectrum and others is used to generate the synthetic image.

#ifndef SRC_GUI_CLASS_SPECTRUM_ROW_H_
#define SRC_GUI_CLASS_SPECTRUM_ROW_H_

#include <QPushButton>
#include <QString>
#include <QWidget>

#include "gui/spectrum_widget.h"

namespace hsi_data_generator {

class ClassSpectrumRow : public QWidget {
  Q_OBJECT

 public:
  ClassSpectrumRow(const QString& class_name, const int num_bands);

  // Updates the number of bands displayed by the SpectrumWidget.
  void SetNumberOfBands(const int num_bands);

 private slots:  // NOLINT
  void EditButtonPressed();

 private:
  // This is the spectrum widget, which displays the spectrum itself, or allows
  // the user to edit it.
  SpectrumWidget* spectrum_widget_ = nullptr;

  // The "Edit" button (changes to the "Save" button in edit mode) toggles the
  // spectrum widget between editing and rendering mode.
  QPushButton* spectrum_edit_button_ = nullptr;
};

}  // namespace hsi_data_generator

#endif  // SRC_GUI_CLASS_SPECTRUM_ROW_H_
