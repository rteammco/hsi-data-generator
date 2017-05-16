#include "hsi/image_layout.h"

#include <QtDebug>
#include <QtGlobal>  // qrand()

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
  const int width = GetWidth();
  const int height = GetHeight();
  int num_pixels_remaining = spectral_class_map_.size();
  std::vector<bool> filled_in_pixels(num_pixels_remaining, false);
  while (num_pixels_remaining > 0) {
    const int current_class = qrand() % num_classes;
    int start_index = qrand() % num_pixels_remaining;
    int initial_start_index = start_index;
    while (filled_in_pixels[start_index]) {
      start_index++;
      if (start_index >= filled_in_pixels.size()) {
        start_index = 0;
      }
      if (start_index == initial_start_index) {
        return;
      }
    }
    spectral_class_map_[start_index] = current_class;
    filled_in_pixels[start_index] = true;
    std::vector<int> edge_pixels;
    edge_pixels.push_back(start_index);
    //qInfo() << "Start index =" << start_index << "(class" << current_class << ")";
    for (int i = 0; i < random_blob_size; ++i) {
      if (num_pixels_remaining <= 0) {
        break;
      }
      const int edge_index = qrand() % edge_pixels.size();
      const int expand_index = edge_pixels[edge_index];
      const int expand_row = expand_index / width;
      const int expand_col = expand_index % width;
      //qInfo() << "Edge selected =" << expand_index << "(" << expand_row << "," << expand_col << ")";
      std::vector<int> neighbor_candidates;
      if (expand_col > 0) {  // left
        const int neighbor_index =  expand_row * width + (expand_col - 1);
        if (!filled_in_pixels[neighbor_index]) {
          neighbor_candidates.push_back(neighbor_index);
        }
      }
      if (expand_col < (width - 1)) {  // right
        const int neighbor_index =  expand_row * width + (expand_col + 1);
        if (!filled_in_pixels[neighbor_index]) {
          neighbor_candidates.push_back(neighbor_index);
        }
      }
      if (expand_row > 0) {  // top
        const int neighbor_index =  (expand_row - 1) * width + expand_col;
        if (!filled_in_pixels[neighbor_index]) {
          neighbor_candidates.push_back(neighbor_index);
        }
      }
      if (expand_row < (height - 1)) {  // bottom
        const int neighbor_index =  (expand_row + 1) * width + expand_col;
        if (!filled_in_pixels[neighbor_index]) {
          neighbor_candidates.push_back(neighbor_index);
        }
      }
      //qInfo() << "Candidates are:";
      for (const int candidate : neighbor_candidates) {
        //qInfo() << "    " << candidate;
      }
      if (neighbor_candidates.size() == 0) {
        edge_pixels.erase(edge_pixels.begin() + edge_index);
        //qInfo() << "    NONE";
      } else {
        const int neighbor_selection = qrand() % neighbor_candidates.size();
        const int neighbor_index = neighbor_candidates[neighbor_selection];
        spectral_class_map_[neighbor_index] = current_class;
        filled_in_pixels[neighbor_index] = true;
        num_pixels_remaining -= 1;
        //qInfo() << "Selected: " << neighbor_selection << " (" << neighbor_index << ")";
      }
    }
    //break;
  }

//  for (int row = 0; row < image_height_; ++row) {
//    for (int col = 0; col < image_width_; ++col) {
//      const int index = row * image_width_ + col;
//      spectral_class_map_[index] = qrand() % num_classes;
//    }
//  }
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
