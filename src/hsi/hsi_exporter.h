// The HSIDataExporter handles creating and saving the final hyperspectral
// image data cube to a binary ENVI file. It combines the individual spectra
// and the image layout to generate the large output data file. It can also
// handle adding noise and other "final touches" to the generated data.

#ifndef SRC_HSI_HSI_EXPORTER_H_
#define SRC_HSI_HSI_EXPORTER_H_

#include <QString>

#include <memory>
#include <vector>

#include "hsi/image_layout.h"
#include "hsi/spectrum.h"

namespace hsi_data_generator {

class HSIDataExporter {
 public:
  HSIDataExporter(
      const std::shared_ptr<std::vector<std::shared_ptr<Spectrum>>> spectra,
      const std::shared_ptr<ImageLayout> image_layout,
      const int num_bands)
      : spectra_(spectra), image_layout_(image_layout), num_bands_(num_bands) {}

  // Saves the file to the given file path. This will be a binary ENVI file.
  // An additional header file will also be saved, which will have the same
  // name but with a ".hdr" extension.
  //
  // Returns true on success.
  bool SaveFile(const QString& file_name) const;

  // Returns any error message caused by SaveFile(). If no errors were logged,
  // returns a generic error string.
  QString GetErrorMessage() const;

 private:
  const std::shared_ptr<std::vector<std::shared_ptr<Spectrum>>> spectra_;
  const std::shared_ptr<ImageLayout> image_layout_;

  // The number of bands that will be exported in the HSI image.
  const int num_bands_;

  // This error message is logged if the SaveFile operation fails.
  mutable QString error_message_;
};

}  // namespace hsi_data_generator

#endif  // SRC_HSI_HSI_EXPORTER_H_
