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
      std::shared_ptr<std::vector<std::shared_ptr<Spectrum>>> spectra,
      std::shared_ptr<ImageLayout> image_layout,
      std::shared_ptr<int> num_bands)
      : spectra_(spectra), image_layout_(image_layout), num_bands_(num_bands) {}

  bool SaveProjectToFile(const QString& file_name) const;

  bool LoadProjectFromFile(const QString& file_name);

  // Returns any error message caused by the save or load methods. If no errors
  // were logged, returns a generic error string.
  QString GetErrorMessage() const;

 private:
  std::shared_ptr<std::vector<std::shared_ptr<Spectrum>>> spectra_;
  std::shared_ptr<ImageLayout> image_layout_;
  std::shared_ptr<int> num_bands_;

  // This error message is logged if any of the file operations fails.
  mutable QString error_message_;
};

}  // namespace hsi_data_generator

#endif  // SRC_HSI_PROJECT_LOADER_H_
