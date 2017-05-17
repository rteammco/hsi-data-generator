#include "hsi/image_layout.h"

#include <QtGlobal>  // qrand()

#include <algorithm>
#include <utility>
#include <vector>

namespace hsi_data_generator {
namespace {

// This is the default stripe width in pixels for generating the stripe and
// grid layouts. The actual assigned stripe width can be smaller if the given
// number of classes cannot fit into the width of the image unless each stripe
// is thinner than this value.
constexpr int kDefaultMaxStripeWidth = 25;

static const std::vector<std::pair<int, int>> kCoordinateNeighborOffsets = {
    std::make_pair(0, -1),  // left
    std::make_pair(0, 1),   // right
    std::make_pair(-1, 0),  // top
    std::make_pair(1, 0),   // bottom
};

}  // namespace

ImageLayout::ImageLayout(const int image_width, const int image_height)
    : image_width_(image_width), image_height_(image_height) {

  // All pixels will be mapped to 0 (the default class index) initially.
  spectral_class_map_.resize(image_width_ * image_height_);
}

void ImageLayout::GenerateHorizontalStripesLayout(
    const int num_classes, const int stripe_width) {

  int pixels_per_class = stripe_width;
  if (stripe_width <= 0) {
    pixels_per_class =
        std::min(image_height_ / num_classes, kDefaultMaxStripeWidth);
  }
  for (int row = 0; row < image_height_; ++row) {
    const int class_index = (row / pixels_per_class) % num_classes;
    std::fill(
        spectral_class_map_.begin() + (row * image_width_),
        spectral_class_map_.begin() + ((row + 1) * image_width_),
        class_index);
  }
}

void ImageLayout::GenerateVerticalStripesLayout(
    const int num_classes, const int stripe_width) {

  int pixels_per_class = stripe_width;
  if (stripe_width <= 0) {
    pixels_per_class =
        std::min(image_width_ / num_classes, kDefaultMaxStripeWidth);
  }
  for (int col = 0; col < image_width_; ++col) {
    const int class_index = (col / pixels_per_class) % num_classes;
    for (int row = 0; row < image_height_; ++row) {
      const int index = row * image_width_ + col;
      spectral_class_map_[index] = class_index;
    }
  }
}

void ImageLayout::GenerateGridLayout(
    const int num_classes, const int square_width) {

  int pixels_per_class = square_width;
  if (square_width <= 0) {
    pixels_per_class =
        std::min(image_width_ / num_classes, kDefaultMaxStripeWidth);
  }
  for (int row = 0; row < image_height_; ++row) {
    for (int col = 0; col < image_width_; ++col) {
      const int index = row * image_width_ + col;
      const int class_index =
          ((row / pixels_per_class) + (col / pixels_per_class)) % num_classes;
      spectral_class_map_[index] = class_index;
    }
  }
}

void ImageLayout::GenerateRandomLayout(
    const int num_classes, const int random_blob_size) {

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
}

int ImageLayout::GetClassAtPixel(const int x_col, const int y_row) const {
  const int map_index = GetMapIndex(x_col, y_row);
  // TODO: Some range checking?
  return spectral_class_map_[map_index];
}

int ImageLayout::GetMapIndex(const int x_col, const int y_row) const {
  // TODO: Some range checking?
  return y_row * image_width_ + x_col;
}

}  // namespace hsi_data_generator
