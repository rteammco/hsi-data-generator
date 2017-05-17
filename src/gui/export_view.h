// This widget provides a GUI for putting finishing touches on the generated
// image. These additional modifications will allow the user to blur, add
// noise, and do other distortions to the data to make it a more challenging
// evaluation data set.

#ifndef SRC_GUI_EXPORT_VIEW_H_
#define SRC_GUI_EXPORT_VIEW_H_

#include <QWidget>

#include <memory>
#include <vector>

#include "hsi/image_layout.h"
#include "hsi/spectrum.h"

namespace hsi_data_generator {

class ExportView : public QWidget {
  Q_OBJECT

 public:
  ExportView(
      std::shared_ptr<int> num_bands,
      std::shared_ptr<std::vector<std::shared_ptr<Spectrum>>> spectra,
      std::shared_ptr<ImageLayout> image_layout);

 private slots:  // NOLINT
  void ExportButtonPressed();

 private:
  std::shared_ptr<int> num_bands_;
  std::shared_ptr<std::vector<std::shared_ptr<Spectrum>>> spectra_;
  std::shared_ptr<ImageLayout> image_layout_;
};

}  // namespace hsi_data_generator

#endif  // SRC_GUI_EXPORT_VIEW_H_
