#include "hsi/hsi_exporter.h"

#include <QString>

#include <fstream>

#include "util/util.h"

namespace hsi_data_generator {
namespace {

// Error messages for various file operations:
static const QString kSubstitutePlaceholder = "%";

static const QString kGenericErrorMessage =
    "Sorry. I derped trying to save the file.";

static const QString kNotEnoughSpectraErrorMessage =
    "Could not save file: at least one spectrum must be available.";

static const QString kInvalidNumberOfBandsErrorMessage =
    "Invalid number of spectral bands: must be between " +
    QString::number(util::kMinNumberOfBands) + " and " +
    QString::number(util::kMaxNumberOfBands) + ".";

static const QString kInvalidImageSizeErrorMessage =
    "Invalid image dimensions: width and height must be between " +
    QString::number(util::kMinImageDimensionSize) + " and " +
    QString::number(util::kMaxImageDimensionSize) + ".";

static const QString kFileNotOpenErrorMessage =
    "Could not open file \"" + kSubstitutePlaceholder + "\" for writing.";

}  // namespace

bool HSIDataExporter::SaveFile(const QString& file_name) const {
  // Determine variables and check for validity:
  if (spectra_->size() < 1) {
    error_message_ = kNotEnoughSpectraErrorMessage;
    return false;
  }
  if (num_bands_ < util::kMinNumberOfBands ||
      num_bands_ > util::kMaxNumberOfBands) {
    error_message_ = kInvalidNumberOfBandsErrorMessage;
    return false;
  }
  const int num_rows = image_layout_->GetHeight();
  if (num_rows < util::kMinImageDimensionSize ||
      num_rows > util::kMaxImageDimensionSize) {
    error_message_ = kInvalidImageSizeErrorMessage;
    return false;
  }
  const int num_cols = image_layout_->GetWidth();
  if (num_cols < util::kMinImageDimensionSize ||
      num_cols > util::kMaxImageDimensionSize) {
    error_message_ = kInvalidImageSizeErrorMessage;
    return false;
  }
  const int data_size = sizeof(float);  // TODO: Define type elsewhere?
  // TODO: Endian format?
  // TODO: Interleave format (BQS, BIL, BIP)?

  // Write the file:
  std::ofstream data_file(file_name.toStdString());
  if (!data_file.is_open()) {
    error_message_ = kFileNotOpenErrorMessage;
    error_message_.replace(kSubstitutePlaceholder, file_name);
    return false;
  }
  for (int band = 0; band < num_bands_; ++band) {
    for (int row = 0; row < num_rows; ++row) {
      for (int col = 0; col < num_cols; ++col) {
        // TODO: Write the data value here.
      }
    }
  }
  data_file.close();
  return true;
}

QString HSIDataExporter::GetErrorMessage() const {
  if (error_message_.isEmpty()) {
    return kGenericErrorMessage;
  }
  return error_message_;
}

}  // namespace hsi_data_generator
