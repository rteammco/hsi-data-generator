// This widget displays and allows editing of a single class spectrum. This
// spectrum and others is used to generate the synthetic image.

#ifndef SRC_GUI_CLASS_SPECTRUM_ROW_H_
#define SRC_GUI_CLASS_SPECTRUM_ROW_H_

#include <QColor>
#include <QPushButton>
#include <QString>
#include <QWidget>

#include "gui/color_box_widget.h"
#include "gui/spectrum_widget.h"

namespace hsi_data_generator {

class ClassSpectrumRow : public QWidget {
  Q_OBJECT

 public:
  ClassSpectrumRow(const int num_bands, Spectrum* spectrum);

  // Updates the number of bands displayed by the SpectrumWidget.
  void SetNumberOfBands(const int num_bands);

  // TODO: Temporary!
  QColor GetClassColor() const { return Qt::black; }
  QString GetClassName() const { return "TEMPORARY"; }

 private slots:  // NOLINT
  void ClassNameFieldChanged(const QString& text);
  void EditButtonPressed();
  void ClearButtonPressed();

 private:
  // The Spectrum itself is passed in and edited by the SpectrumWidget
  // contained in this ClassSpectrumRow widget.
  Spectrum* spectrum_;

  // This is a small box that just displays the color of the spectrum class.
  // The color is adjustable by the user by clicking on it.
  ColorBoxWidget* class_color_box_ = nullptr;

  // This is the spectrum widget, which displays the spectrum itself, or allows
  // the user to edit it.
  SpectrumWidget* spectrum_widget_ = nullptr;

  // The "Edit" button (changes to the "Save" button in edit mode) toggles the
  // spectrum widget between editing and rendering mode.
  QPushButton* spectrum_edit_button_ = nullptr;
};

}  // namespace hsi_data_generator

#endif  // SRC_GUI_CLASS_SPECTRUM_ROW_H_
