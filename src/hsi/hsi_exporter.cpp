#include "hsi/hsi_exporter.h"

#include <QString>
#include <QtDebug>

#include <fstream>
#include <vector>

#include "util/util.h"

namespace hsi_data_generator {
namespace {

// The extension appended on the file name for writing header files:
static const QString kHeaderFileExtension = ".hdr";

// Error messages for various file operations:
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
    "Could not open file \"" + util::kTextSubPlaceholder + "\" for writing.";

static const QString kInvalidSpectrumClassErrorMessage =
    "Invalid spectrum class: must be between 0 and " +
    util::kTextSubPlaceholder + ".";

}  // namespace

bool HSIDataExporter::SaveFile(const QString& file_name) const {
  // Determine variables and check for validity:
  const int num_spectra = spectra_->size();
  if (num_spectra < 1) {
    error_message_ = kNotEnoughSpectraErrorMessage;
    return false;
  }
  std::vector<std::vector<double>> generated_spectra;
  for (int i = 0; i < num_spectra; ++i) {
    generated_spectra.push_back(spectra_->at(i)->GenerateSpectrum(num_bands_));
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
    error_message_ = util::ReplaceTextSubPlaceholder(
        kFileNotOpenErrorMessage, file_name);
    return false;
  }
  // BSQ (band sequential) format:
  for (int band = 0; band < num_bands_; ++band) {
    for (int row = 0; row < num_rows; ++row) {
      for (int col = 0; col < num_cols; ++col) {
        const int class_index = image_layout_->GetClassAtPixel(col, row);
        if (class_index < 0 || class_index >= num_spectra) {
          error_message_ = util::ReplaceTextSubPlaceholder(
              kInvalidSpectrumClassErrorMessage,
              QString::number(num_spectra - 1));
          data_file.close();
          return false;
        }
        const double data_value = generated_spectra[class_index][band];
        const float export_value = static_cast<float>(data_value);
        data_file.write(
            reinterpret_cast<const char*>(&export_value), data_size);
      }
    }
  }
  data_file.close();

  // Now write the header file:
  const QString header_file_name = file_name + kHeaderFileExtension;
  std::ofstream header_file(header_file_name.toStdString());
  if (!header_file.is_open()) {
    error_message_ = util::ReplaceTextSubPlaceholder(
        kFileNotOpenErrorMessage, header_file_name);
    return false;
  }
  // TODO: Adjust these header values as needed.
  header_file << "interleave      = bsq\n";
  header_file << "data type       = float\n";
  header_file << "byte order      = 0\n";
  header_file << "header offset   = 0\n";
  header_file << "samples         = " << num_rows << "\n";
  header_file << "lines           = " << num_cols << "\n";
  header_file << "bands           = " << num_bands_ << "\n";
  header_file.close();

  return true;
}

QString HSIDataExporter::GetErrorMessage() const {
  if (error_message_.isEmpty()) {
    return kGenericErrorMessage;
  }
  return error_message_;
}

}  // namespace hsi_data_generator
