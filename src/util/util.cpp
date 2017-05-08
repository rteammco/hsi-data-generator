#include "util/util.h"

#include <QDir>
#include <QFile>
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

QString GetStylesheetRelativePath(const QString& stylesheet_relative_path) {
  const QString stylesheet_absolute_path =
      GetAbsolutePath(stylesheet_relative_path);
  QFile stylesheet_file(stylesheet_absolute_path);
  if (!stylesheet_file.open(QFile::ReadOnly | QFile::Text)) {
    qWarning() << "Could not open stylesheet file "
               << stylesheet_absolute_path
               << ": returning empty stylesheet string.";
    return "";
  }
  return QLatin1String(stylesheet_file.readAll());
}

}  // namespace util
}  // namespace hsi_data_generator
