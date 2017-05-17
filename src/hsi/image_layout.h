// The ImageLayout class maps pixels in the image to different spectrum
// classes, keeping track of the 2D hyperspectral image layout. Once the layout
// is completed and all the spectra are edited, the 2D ImageLayout can be
// combined with each spectrum to generate the full 3D hyperspectral data cube.

#ifndef SRC_HSI_IMAGE_LAYOUT_H_
#define SRC_HSI_IMAGE_LAYOUT_H_

#include <vector>

namespace hsi_data_generator {

class ImageLayout {
 public:
  ImageLayout(const int image_width, const int image_height);

  // TODO: Add a Resize() method that will re-interpolate the image class map
  // to a new width and height.

  // TODO: Add the option to modify individual pixels, so that the GUI can
  // allow manual user editing.

  // Generates a stripe pattern for the given number of spectral classes.
  //
  // This generates a pattern of horizontal stripes, alternating by class, from
  // the top to the bottom of the image.
  //
  // Set stripe_width (>= 1) to force each stripe to be a specific width (in
  // pixels). A value of 0 will automatically set the size to a default value
  // based on the image size and number of classes.
  void GenerateHorizontalStripesLayout(
      const int num_classes, const int stripe_width = 0);

  // This generates a pattern of vertical stripes, alternating by class, from
  // the left to the right of the image.
  //
  // stripe_width behaves the same was as in GenerateHorizontalStripesLayout().
  void GenerateVerticalStripesLayout(
      const int num_classes, const int stripe_width = 0);

  // This generates a grid pattern, where the classes alternate in both the
  // horizontal and vertical directions.
  //
  // square_width behaves the same way as stripe_width in the functions above.
  void GenerateGridLayout(const int num_classes, const int square_width = 0);

  // This generates a random layout, where each blob of pixels is assigned a
  // class label with a uniformly random probability.
  //
  // The random_blob_size (>= 1) argument dictates how many pixels of the same
  // class will be nex to each other in each random blob.
  //
  // A value of 1 means the image will be completely random noise. This is not
  // recommended for most test applications, since total randomness voids any
  // spatial correlations in the image.
  void GenerateRandomLayout(
      const int num_classes, const int random_blob_size = 1);

  // Returns the width in pixels (number of columns) in the image.
  int GetWidth() const {
    return image_width_;
  }

  // Returns the height in pixels (number of rows) in the image.
  int GetHeight() const {
    return image_height_;
  }

  // Returns the total number of pixels in this image layout.
  int GetNumPixels() const {
    return image_width_ * image_height_;
  }

  // Used for referencing the layout externally.
  const std::vector<int>& GetClassMap() const {
    return spectral_class_map_;
  }

  // Returns the value at the given index.
  int GetClassAtPixel(const int x_col, const int y_row) const;

  // Returns the 1D index (into the vector returned by GetClassMap() from a
  // given (X = col, Y = row) 2D image coordinate.
  int GetMapIndex(const int x_col, const int y_row) const;

 private:
  // The spatial dimensions of the hyperspectral image.
  int image_width_;
  int image_height_;

  // This map identifies each pixel in the image as one of the spectral
  // classes. The class indices start at 0 to indicate the first spectrum
  // class.
  std::vector<int> spectral_class_map_;
};

}  // namespace hsi_data_generator

#endif  // SRC_HSI_IMAGE_LAYOUT_H_
