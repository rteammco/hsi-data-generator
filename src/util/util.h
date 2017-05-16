#ifndef SRC_UTIL_UTIL_H_
#define SRC_UTIL_UTIL_H_

#include <QString>

namespace hsi_data_generator {
namespace util {

// Size limits to avoid excessively massive file sizes and memory usage, or
// generally invalid sizes:
constexpr int kMinImageDimensionSize = 1;
constexpr int kMaxImageDimensionSize = 10000;
constexpr int kMinNumberOfBands = 1;
constexpr int kMaxNumberOfBands = 10000;

// Returns the root directory where this project was compiled. This uses the
// root path preprocessor macro specified by CMake. If for some reason this
// flag isn't defined, a warning will be logged and the local directory (".")
// will be returned instead.
QString GetRootCodeDirectory();

// Returns the absolute path of the given relative path by using the root code
// directory (returned by GetRootCodeDirectory()).
QString GetAbsolutePath(const QString& relative_path);

// Reads the given Qt stylesheet file (typically a *.qss file) and returns the
// contents as a string.
//
// This requires a RELATIVE PATH and will use GetRootCodeDirectory() to aquire
// the base code path. For example, if all Qt stylesheets are stored in
// "qt_stylesheets" in the root project directory, specify the path
// "qt_stylesheets/my_style.qss" and the absolute path will be automatically
// resolved.
QString GetStylesheetRelativePath(const QString& stylesheet_relative_path);

}  // namespace util
}  // namespace hsi_data_generator

#endif  // SRC_UTIL_UTIL_H_
