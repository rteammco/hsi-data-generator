cmake_minimum_required(VERSION 2.8)

project(HSIDataGenerator)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Include Qt.
find_package(Qt5Widgets)

# Default to Release mode.
IF(NOT DEFINED CMAKE_BUILD_TYPE)
  SET(${CMAKE_BUILD_TYPE} Release ... FORCE)
ENDIF()

# Release mode makes the code run faster.
IF(NOT ${CMAKE_BUILD_TYPE} MATCHES Debug)
  MESSAGE("Build type: Release.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -Ofast")
ELSE()
  MESSAGE("WARNING: Building in DEBUG MODE! Use 'cmake -DCMAKE_BUILD_TYPE=Release' for faster runtime.")
ENDIF()

# Require C++ 11.
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Define the code root directory for use as a defined macro in the code.
add_definitions(-DROOT_CODE_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}")

# Libraries will be stored in the "lib" directory, and binaries in "bin".
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Grab all binary source files in the src directory.
file(GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*")

# Other source files:
file(GLOB   gui_SRC    "src/gui/*.cpp")
file(GLOB   hsi_SRC    "src/hsi/*.cpp")
file(GLOB   util_SRC   "src/util/*.cpp")

# This is for Qt to allow using the Q_OBJECT macro.
set(CMAKE_AUTOMOC TRUE)

# Add the app binary.
add_executable(
  HSIDataGenerator
  src/hsi_data_generator.cpp
  ${gui_SRC}
  ${hsi_SRC}
  ${util_SRC}
)
target_link_libraries(
  HSIDataGenerator
  Qt5::Widgets
)
