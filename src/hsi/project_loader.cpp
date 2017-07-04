#include "hsi/project_loader.h"

#include <QString>

#include <fstream>
#include <vector>

#include "hsi/spectrum.h"

namespace hsi_data_generator {
namespace {

static const QString kProjectFileHeader =
    "Project File Exported by HSIDataGenerator.";

static const QString kSubstitutePlaceholder = "%";

static const QString kGenericErrorMessage =
    "File read/write error.";
static const QString kFileNotOpenErrorMessage =
    "Could not open file \"" + kSubstitutePlaceholder + "\" for writing.";

}  // namespace

bool ProjectLoader::SaveProjectToFile(const QString& file_name) const {
  std::ofstream project_file(file_name.toStdString());
  if (!project_file.is_open()) {
    error_message_ = kFileNotOpenErrorMessage;
    error_message_.replace(kSubstitutePlaceholder, file_name);
    return false;
  }
  project_file << kProjectFileHeader.toStdString() << "\n\n";
  project_file << "num_bands = " << num_bands_ << "\n";
  // Save the spectra information in the following format:
  //   spectrum = {
  //     name = Name of Spectrum
  //     color = #12345f
  //     peaks = {
  //       position, amplitude, width
  //       0.32 0.15 0.58
  //     }
  //   }
  //   spectrum = {
  //     ...
  //   }
  for (int i = 0; i < spectra_->size(); ++i) {
    const std::shared_ptr<Spectrum> spectrum = spectra_->at(i);
    const QString spectrum_name = spectrum->GetName();
    const QString spectrum_color = spectrum->GetColor().name();
    project_file << "spectrum = {\n";
    project_file << "  name = " << spectrum_name.toStdString() << "\n";
    project_file << "  color = " << spectrum_color.toStdString() << "\n";
    project_file << "  peaks = {\n";
    for (const PeakDistribution& peak : spectrum->GetPeaks()) {
      project_file << "    " << peak.position << " " << peak.amplitude << " "
                   << peak.width << "\n";
    }
    project_file << "  }\n";
    project_file << "}\n";
  }
  // TODO: Save the layout.
  // Save the layout information in the following format:
  // Layout:
  // layout_width = 100
  // layout_height = 100
  // layout {
  //   row 1
  //   0 0 0 0 0 1 1 1 2 2 2 1 1 1 0 0 1 ... 0 1 0
  //   1 2 2 2 2 2 1 1 2 3 3 4 5 5 5 5 ....
  //   ...
  //   row 100
  // }
  project_file.close();
  return true;
}

bool ProjectLoader::LoadProjectFromFile(const QString& file_name) {
  // TODO: Implement;
  return false;
}

QString ProjectLoader::GetErrorMessage() const {
  if (error_message_.isEmpty()) {
    return kGenericErrorMessage;
  }
  return error_message_;
}

}  // namespace hsi_data_generator
