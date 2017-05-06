#include "util/util.h"

#include <QDir>
#include <QFileInfo>
#include <QString>
#include <QtDebug>

namespace hsi_data_generator {
namespace util {

QString GetRootCodeDirectory() {
#ifdef ROOT_CODE_DIRECTORY
  return QString(ROOT_CODE_DIRECTORY);
#else
  qWarning() << "ROOT_CODE_DIRECTORY is not defined. "
             << "Returning local Unix directory ('.')";
  return ".";
#endif
}

QString GetAbsolutePath(const QString& relative_path) {
  const QString root_directory = GetRootCodeDirectory();
  QFileInfo file_info(QDir(root_directory), relative_path);
  return file_info.absoluteFilePath();
}

}  // namespace util
}  // namespace hsi_data_generator
