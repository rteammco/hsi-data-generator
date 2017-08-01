#include "hsi/image_layout.h"

#include <QImage>
#include <QColor>
#include <QtGlobal>  // qrand()
#include <QtDebug>

#include <algorithm>
#include <cmath>
#include <limits>
#include <utility>
#include <vector>

namespace hsi_data_generator {
namespace {

// This is the default stripe width for generating the stripe and grid layouts.
// The actual assigned stripe width can be smaller if the given number of
// classes cannot fit into the width of the image unless each stripe is thinner
// than this value.
constexpr double kDefaultMaxPrimitiveSize = 0.1;

static const std::vector<std::pair<int, int>> kCoordinateNeighborOffsets = {
    std::make_pair(0, -1),  // left
    std::make_pair(0, 1),   // right
    std::make_pair(-1, 0),  // top
    std::make_pair(1, 0),   // bottom
};

// Returns the appropriate width or height for a single layout stripe
// primitive.  If the given input size is within valid range, it will just
// return that.  Otherwise the default size will be returned. If that is too
// large (i.e. the number of classes is too many for this size to acomodate all
// of them per row/column, the size will be shrunk to fit all the classes.
double GetAppropriateShapeSize(
    const double input_shape_size, const int num_classes) {

  if (input_shape_size > 0.0 && input_shape_size <= 1.0) {
    return input_shape_size;
  }

  const double shape_size = kDefaultMaxPrimitiveSize;
  const double min_shape_size = 1.0 / static_cast<double>(num_classes);
  if (shape_size > min_shape_size) {
    return min_shape_size;
  }
  return shape_size;
}

}  // namespace

ImageLayout::ImageLayout(const int image_width, const int image_height)
    : image_width_(image_width), image_height_(image_height) {

  // All pixels will be mapped to 0 (the default class index) initially.
  // TODO: Fill with 0 or a "non-class" id, such as -1?
  spectral_class_map_.resize(GetNumPixels());
}

void ImageLayout::AddSubLayout(
    const double left_x,
    const double top_y,
    const double width,
    const double height,
    ImageLayout layout) {

  const LayoutComponentShape component_shape(left_x, top_y, width, height);
  sub_layouts_.push_back(std::make_pair(component_shape, layout));
}

void ImageLayout::AddLayoutPrimitive(
    const double left_x,
    const double top_y,
    const double width,
    const double height,
    const int spectral_class) {

  const LayoutComponentShape component_shape(left_x, top_y, width, height);
  layout_primitives_.push_back(std::make_pair(component_shape, spectral_class));
}

void ImageLayout::GenerateHorizontalStripesLayout(
    const int num_classes, const double stripe_size) {

  ResetLayout();
  const double stripe_height =
      GetAppropriateShapeSize(stripe_size, num_classes);
  int stripe_counter = 0;
  double height_filled = 0.0;
  while ((1.0 - height_filled) > std::numeric_limits<double>::epsilon()) {
    const int spectral_class = stripe_counter % num_classes;
    AddLayoutPrimitive(0.0, height_filled, 1.0, stripe_height, spectral_class);
    height_filled += stripe_height;
    stripe_counter++;
  }
  Render();
}

void ImageLayout::GenerateVerticalStripesLayout(
    const int num_classes, const double stripe_size) {

  ResetLayout();
  const double stripe_width =
      GetAppropriateShapeSize(stripe_size, num_classes);
  int stripe_counter = 0;
  double width_filled = 0.0;
  while ((1.0 - width_filled) > std::numeric_limits<double>::epsilon()) {
    const int spectral_class = stripe_counter % num_classes;
    AddLayoutPrimitive(width_filled, 0.0, stripe_width, 1.0, spectral_class);
    width_filled += stripe_width;
    stripe_counter++;
  }
  Render();
}

void ImageLayout::GenerateGridLayout(
    const int num_classes, const double input_square_size) {

  ResetLayout();
  const double square_size =
      GetAppropriateShapeSize(input_square_size, num_classes);
  int row_counter = 0;
  double height_filled = 0.0;
  while ((1.0 - height_filled) > std::numeric_limits<double>::epsilon()) {
    int col_counter = row_counter;
    double width_filled = 0.0;
    while ((1.0 - width_filled) > std::numeric_limits<double>::epsilon()) {
      const int spectral_class = col_counter % num_classes;
      AddLayoutPrimitive(
          width_filled,
          height_filled,
          square_size,
          square_size,
          spectral_class);
      width_filled += square_size;
      col_counter++;
    }
    height_filled += square_size;
    row_counter += num_classes / 2;
  }
  Render();
}

void ImageLayout::GenerateRandomLayout(
    const int num_classes, const int random_blob_size) {

  // TODO: Fix to new standards.
  /*
  const int width = GetWidth();
  const int height = GetHeight();
  int num_pixels_remaining = spectral_class_map_.size();
  std::vector<bool> filled_in_pixels(num_pixels_remaining, false);
  while (num_pixels_remaining > 0) {
    // Select a random class for the next blob:
    const int current_class = qrand() % num_classes;
    // Select a random pixel that isn't part of a blob:
    int start_index = qrand() % num_pixels_remaining;
    while (filled_in_pixels[start_index]) {
      start_index++;
      if (start_index >= filled_in_pixels.size()) {
        start_index = 0;
      }
    }
    // Fill in the randomly selected pixel:
    spectral_class_map_[start_index] = current_class;
    num_pixels_remaining--;
    filled_in_pixels[start_index] = true;
    // Add this pixel as the only initial edge pixel in the blob:
    std::vector<int> edge_pixels;
    edge_pixels.push_back(start_index);
    // Fill in pixels for the rest of the blob:
    for (int i = 1; i < random_blob_size; ++i) {
      // Stop if there no more available edges or if all pixels were filled in.
      if (num_pixels_remaining <= 0 || edge_pixels.empty()) {
        break;
      }
      // Randomly select one of the blob's edge pixels:
      const int edge_index = qrand() % edge_pixels.size();
      const int expand_index = edge_pixels[edge_index];
      const int expand_row = expand_index / width;
      const int expand_col = expand_index % width;
      // Select all valid candidate pixels adjacent (left, right, top, or
      // bottom) to the selected egde pixel to be filled in next. A candidate
      // is only valid if it is inside the bounds of the image and if it hasn't
      // already been filled in before.
      std::vector<int> neighbor_candidates;
      for (const std::pair<int, int>& offset : kCoordinateNeighborOffsets) {
        const int row = expand_row + offset.first;
        if (row < 0 || row >= height) {
          continue;
        }
        const int col = expand_col + offset.second;
        if (col < 0 || col >= width) {
          continue;
        }
        const int neighbor_index =  row * width + col;
        if (!filled_in_pixels[neighbor_index]) {
          neighbor_candidates.push_back(neighbor_index);
        }
      }
      // If no neighbors are available, this edge pixel has no valid neighbors,
      // so remove it from the set of neighbors.
      if (neighbor_candidates.size() == 0) {
        edge_pixels.erase(edge_pixels.begin() + edge_index);
      } else {
        // Randomly select one of the neighbors and fill it in as part of the
        // blob. This new filled-in pixel becomes a new edge pixel.
        const int neighbor_index =
            neighbor_candidates[qrand() % neighbor_candidates.size()];
        spectral_class_map_[neighbor_index] = current_class;
        filled_in_pixels[neighbor_index] = true;
        edge_pixels.push_back(neighbor_index);
        num_pixels_remaining -= 1;
      }
    }
  }
  */
}

void ImageLayout::GenerateLayoutFromImage(
    const int num_classes, const QImage& layout_image) {

  // TODO: Fix to new standards.
  // Resize the image to fit the layout's size.
  QImage image = layout_image.scaled(GetWidth(), GetHeight());

  // Convert the image to grayscale if it isn't already. Code taken from:
  // https://stackoverflow.com/questions/27949569/convert-a-qimage-to-grayscale
  // Pixel depth is 1 (grayscale), 3 (rgb), or 4 (rgba), etc. QImage.depth is
  // given in bits (8 per byte).
  const int pixel_depth = image.depth() / 8;
  for (int row = 0; row < GetHeight(); ++row) {
    unsigned char* scanline = image.scanLine(row);
    for (int col = 0; col < GetWidth(); ++col) {
      QRgb* rgb_pixel = reinterpret_cast<QRgb*>(scanline + col * pixel_depth);
      const int gray_value = qGray(*rgb_pixel);
      *rgb_pixel = QColor(gray_value, gray_value, gray_value).rgba();
    }
  }
  image = image.convertToFormat(QImage::Format_Indexed8);

  // Set the class map values by pixel intensity.
  // TODO: Possibly, do pixel intensity clustering instead of this hack.
  for (int col = 0; col < GetWidth(); ++col) {
    for (int row = 0; row < GetHeight(); ++row) {
      const int gray_value = qGray(image.pixel(col, row));
      // 256 possible intensity values, remapped into num_classes values.
      const int class_index =
          gray_value / std::ceil(256.0 / static_cast<double>(num_classes));
      spectral_class_map_[GetMapIndex(col, row)] = class_index;
    }
  }
}

void ImageLayout::ResetLayout() {
  // TODO: Fill with 0 or a "non-class" id, such as -1?
  sub_layouts_.clear();
  layout_primitives_.clear();
  std::fill(spectral_class_map_.begin(), spectral_class_map_.end(), 0);
}

void ImageLayout::Render() {
  qInfo() << "Render()," << layout_primitives_.size();
  const int num_pixels = GetNumPixels();
  spectral_class_map_.resize(num_pixels);
  std::fill(
      spectral_class_map_.begin(),
      spectral_class_map_.begin() + num_pixels,
      0);  // TODO: Default class value.
  for (const auto& shape_and_class : layout_primitives_) {
    const LayoutComponentShape& component_shape = shape_and_class.first;
    const int spectral_class = shape_and_class.second;
    const int start_x = static_cast<int>(
        component_shape.left_x * static_cast<double>(GetWidth()));
    const int end_x = start_x + static_cast<int>(
        component_shape.width * static_cast<double>(GetWidth()));
    const int start_y = static_cast<int>(
        component_shape.top_y * static_cast<double>(GetHeight()));
    const int end_y = start_y + static_cast<int>(
        component_shape.height * static_cast<double>(GetHeight()));
    const int start_x_index = std::max(start_x, 0);
    const int end_x_index = std::min(end_x, GetWidth() - 1);
    const int start_y_index = std::max(start_y, 0);
    const int end_y_index = std::min(end_y, GetHeight() - 1);
    for (int x = start_x_index; x < end_x_index; ++x) {
      for (int y = start_y_index; y < end_y_index; ++y) {
        spectral_class_map_[GetMapIndex(x, y)] = spectral_class;
      }
    }
  }
}

void ImageLayout::SetImageSize(const int width, const int height) {
  // TODO: Check width and height validity.
  image_width_ = width;
  image_height_ = height;
  Render();
}

int ImageLayout::GetClassAtPixel(const int x_col, const int y_row) const {
  const int map_index = GetMapIndex(x_col, y_row);
  // TODO: Some range checking?
  return spectral_class_map_[map_index];
}

int ImageLayout::GetMapIndex(const int x_col, const int y_row) const {
  // TODO: Some range checking?
  return y_row * GetWidth() + x_col;
}

}  // namespace hsi_data_generator
