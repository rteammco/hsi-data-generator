# HSI Data Generator

A tool for generating for artificial hyperspectral image data. Allows the user to define a spectral dictionary and a 2D image layout. This can then be exported to generate a binary HSI file.

## Installation

#### Required Libraries and Tools

<ul>
  <li> Qt 5+ (macOS: <code>brew install qt</code>). QtCreator is not required. </li>
  <li> CMake (version 2.8+). </li>
</ul>

#### Build Instructions

```
git clone https://github.com/teammcr192/hsi-data-generator.git
cd hsi-data-generator
mdkir build
cd build
cmake ..
make
```

#### Common Issues

<ul>
  <li> CMake cannot find Qt on macOS: try running <code>cmake .. -DCMAKE_PREFIX_PATH=/usr/local/Cellar/qt/5.8.0_2/</code> in place of <code>cmake ..</code>. Modify the version as needed. </li>
  <li> Build in release mode: use <code>cmake .. -DCMAKE_BUILD_TYPE=Release</code>. This can make the code run faster.
  <li> If you choose not to use CMake for whatever reason, make sure to define the preprocessor macro <code>ROOT_CODE_DIRECTORY</code> as the root code directory (i.e. <code>/path/to/hsi-data-generator</code>). This is the directory containing the <code>qt_stylesheets</code>, <code>resources</code>, and <code>src</code> directories. Omitting this may cause issues when running the code. </li>
</ul>

## Usage

Run the binary (`bin/HSIDataGenerator`) after building the code.

The GUI is organized into three tabs. An overview is provided below.

#### Class Spectra Tab

This tab allows you to define a spectral dictionary. Each spectrum is a class, and the final image will be constructed as combination of these spectra.

#### Image Layout Tab

This tab allows you to define an image layout, which presents a 2D view of how the spectra will be organized in the final output.

#### Export Tab

This tab is where you can specify any final touches, such as spectral noise that will be added to the data, and save the final binary image. Currently, all images are exported in the following HSI format:

```
interleave      = bsq
data type       = float
byte order      = 0
header offset   = 0
```

<ul>
  <li> Interleave is BSQ (band-sequential). </li>
  <li> All data points are stored as floats. </li>
  <li> Byte order is little endian. </li>
  <li> The header is exported separately, and is not stored as a part of the data file. </li>
</ul>
