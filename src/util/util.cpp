#include "util/util.h"

#include <QDir>
#include <QFileInfo>
#include <QtDebug>

#include <string>

namespace hsi_data_generator {
namespace util {

std::string GetRootCodeDirectory() {
#ifdef ROOT_CODE_DIRECTORY
  return std::string(ROOT_CODE_DIRECTORY);
#else
  qWarning() << "ROOT_CODE_DIRECTORY is not defined. "
             << "Returning local Unix directory ('.')";
  return ".";
#endif
}

std::string GetAbsolutePath(const std::string& relative_path) {
  const std::string root_directory = GetRootCodeDirectory();
  QFileInfo file_info(QDir(root_directory.c_str()), relative_path.c_str());
  const QString path = file_info.absoluteFilePath();
  return path.toStdString();
}

}  // namespace util
}  // namespace hsi_data_generator
