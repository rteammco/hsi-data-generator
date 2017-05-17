// This class is responsible for saving and loading project files. This
// includes layouts and the spectra themselves. The project can be saved so
// that the user can just open it back up and continue where they left off.

#ifndef SRC_HSI_PROJECT_LOADER_H_
#define SRC_HSI_PROJECT_LOADER_H_

#include <QString>

#include <memory>
#include <vector>

#include "hsi/image_layout.h"
#include "hsi/spectrum.h"

namespace hsi_data_generator {

class ProjectLoader {
 public:
  ProjectLoader(
      const std::shared_ptr<std::vector<std::shared_ptr<Spectrum>>> spectra,
      const std::shared_ptr<ImageLayout> image_layout,
      const int num_bands)
      : spectra_(spectra), image_layout_(image_layout), num_bands_(num_bands) {}

  void SaveProjectToFile(const QString& file_name) const;

  void LoadProjectFromFile(const QString& file_name);

 private:
  const std::shared_ptr<std::vector<std::shared_ptr<Spectrum>>> spectra_;
  const std::shared_ptr<ImageLayout> image_layout_;

  // The number of bands that will be exported in the HSI image.
  const int num_bands_;
};

}  // namespace hsi_data_generator

#endif  // SRC_HSI_PROJECT_LOADER_H_
