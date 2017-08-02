// The ImageLayout class stores a 2D layout for the hyperspectral image. Each
// layout can contain rectangles of a single spectral class (primitives) or
// complex sub-layouts that are stored inside this layout. The layout is
// recursively rendered at any given pixel resolution.
//
// The rendered layout maps pixels in the image to different spectrum classes.
// Once the layout is completed and all the spectra are edited, the 2D
// ImageLayout can be combined with each spectrum to generate the full 3D
// hyperspectral data cube.

#ifndef SRC_HSI_IMAGE_LAYOUT_H_
#define SRC_HSI_IMAGE_LAYOUT_H_

#include <QImage>

#include <utility>
#include <vector>

namespace hsi_data_generator {

// A simple struct that keeps track of a rectangle's shape. This is used to
// track sub-layout and layout primitive positions within a root layout.
struct LayoutComponentShape {
  LayoutComponentShape(
      const double left_x,
      const double top_y,
      const double width,
      const double height)
      : left_x(left_x), top_y(top_y), width(width), height(height) {}

  const double left_x;
  const double top_y;
  const double width;
  const double height;
};

class ImageLayout {
 public:
  ImageLayout(const int image_width, const int image_height);

  // Add a sublayout.
  void AddSubLayout(
      const double left_x,
      const double top_y,
      const double width,
      const double height);

  // Add a primitive shape with a single class.
  void AddLayoutPrimitive(
      const double left_x,
      const double top_y,
      const double width,
      const double height,
      const int spectral_class);

  // TODO: Possibly, add the option to modify individual pixels (or manually
  // insert layout primitives), so that the GUI can allow manual user editing.

  // Generates a stripe pattern for the given number of spectral classes.
  //
  // This generates a pattern of horizontal stripes, alternating by class, from
  // the top to the bottom of the image.
  //
  // Set stripe_size (0, 1] to force each stripe to be a specific width (in
  // relative size on a 1x1 grid). A value of 0 will automatically set the size
  // to a default value based on the number of classes.
  void GenerateHorizontalStripesLayout(
      const int num_classes, const double stripe_size = 0.0);

  // This generates a pattern of vertical stripes, alternating by class, from
  // the left to the right of the image.
  //
  // stripe_size behaves the same was as in GenerateHorizontalStripesLayout().
  void GenerateVerticalStripesLayout(
      const int num_classes, const double stripe_size = 0.0);

  // This generates a grid pattern, where the classes alternate in both the
  // horizontal and vertical directions.
  //
  // square_width behaves the same way as stripe_width in the functions above.
  void GenerateGridLayout(
      const int num_classes, const double square_size = 0.0);

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

  // Attempts to generate a layout from the given image file. Different colors
  // and shades of the image will be used to map the different spectral classes
  // to the image texture.
  void GenerateLayoutFromImage(
      const int num_classes, const QImage& layout_image);

  // Resets the layout, re-initializing everything to unassigned.
  void ResetLayout();

  // Attempts to "zoom in" to a sub-layout, which would make all Render and
  // edit operations display and act on that sub-layout instead. This should
  // only be used for editing and visualization.
  //
  // If the given coordinates do not intersect a sub-layout, nothing will
  // happen and this method will return false.
  bool ZoomInToSubLayout(const double x, const double y);

  // Resets the displayed interactive layout to the top-level. That means if it
  // is currently "zoomed in" to a sub-layout, that sub-layout will no longer
  // be shown in full.
  void ZoomOutToRoot();

  // Renders the layout at the current image size, which will assign each pixel
  // to the appropriate spectral class value. This will generate assignments
  // for the spectral class map (see GetClassMap() and GetClassAtPixel()).
  void Render();

  // Updates the image size. This causes the layout to be recomputed for the
  // new image dimensions.
  void SetImageSize(const int width, const int height);

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
    return GetWidth() * GetHeight();
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
  // The spatial dimensions (pixels) of the hyperspectral image when it is
  // rendered.
  int image_width_;
  int image_height_;

  // These lists contain sub-layouts and layout primitives (single-class
  // rectangles) that are rendered together to produce a finalized layout
  // design. All shapes are stored at relative sizes, but when rendered, they
  // are scaled to the appropriate number of pixels based on the layout's width
  // and height.
  std::vector<std::pair<LayoutComponentShape, ImageLayout>> sub_layouts_;
  std::vector<std::pair<LayoutComponentShape, int>> layout_primitives_;

  // This map identifies each pixel in the image as one of the spectral
  // classes. The class indices start at 0 to indicate the first spectrum
  // class.
  std::vector<int> spectral_class_map_;
};

}  // namespace hsi_data_generator

#endif  // SRC_HSI_IMAGE_LAYOUT_H_
