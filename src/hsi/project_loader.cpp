#include "hsi/project_loader.h"

#include <QString>

namespace hsi_data_generator {
namespace {

static const QString kGenericErrorMessage =
    "File I/O general error... IDK WHATS GOING ON!?!?!?!";

}  // namespace

bool ProjectLoader::SaveProjectToFile(const QString& file_name) const {
  // TODO: Implement.
  return false;
}

bool ProjectLoader::LoadProjectFromFile(const QString& file_name) {
  // TODO: Implement;
  return false;
}

QString ProjectLoader::GetErrorMessage() const {
  return kGenericErrorMessage;
}

}  // namespace hsi_data_generator
