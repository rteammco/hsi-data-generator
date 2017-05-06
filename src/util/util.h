#ifndef SRC_UTIL_UTIL_H_
#define SRC_UTIL_UTIL_H_

#include <string>

namespace hsi_data_generator {
namespace util {

// Returns the root directory where this project was compiled. This uses the
// root path preprocessor macro specified by CMake. If for some reason this
// flag isn't defined, a warning will be logged and the local directory (".")
// will be returned instead.
std::string GetRootCodeDirectory();

// Returns the absolute path of the given relative path by using the root code
// directory (returned by GetRootCodeDirectory()).
std::string GetAbsolutePath(const std::string& relative_path);

}  // namespace util
}  // namespace hsi_data_generator

#endif  // SRC_UTIL_UTIL_H_
