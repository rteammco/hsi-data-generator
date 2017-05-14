#include "image/image_layout.h"

#include <QtGlobal>  // qRand()

#include <algorithm>
#include <vector>

namespace hsi_data_generator {
namespace {

// This is the default stripe width in pixels for generating the stripe and
// grid layouts. The actual assigned stripe width can be smaller if the given
// number of classes cannot fit into the width of the image unless each stripe
// is thinner than this value.
constexpr int kDefaultMaxStripeWidth = 25;

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

  // TODO: Implement support for blob size greater than 1!

  for (int row = 0; row < image_height_; ++row) {
    for (int col = 0; col < image_width_; ++col) {
      const int index = row * image_width_ + col;
      spectral_class_map_[index] = qrand() % num_classes;
    }
  }
}

}  // namespace hsi_data_generator
