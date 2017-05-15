#include "hsi/hsi_exporter.h"

#include <QString>

namespace hsi_data_generator {
namespace {

static const QString kGenericErrorMessage =
    "Sorry. I derped trying to save the file.";

}

bool HSIDataExporter::SaveFile(const QString& file_name) {
  // TODO: Implement.
  return false;
}

QString HSIDataExporter::GetErrorMessage() const {
  // TODO: Return the appropriate error message.
  return kGenericErrorMessage;
}

}  // namespace hsi_data_generator
