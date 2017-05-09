#include "spectrum/spectrum_generator.h"

#include <math.h>

#include <algorithm>
#include <cmath>
#include <vector>

namespace hsi_data_generator {
namespace spectrum_generator {
namespace {

// Returns the value at the given point in the Gaussian distribution defined by
// the given mean and variance.
// See https://en.wikipedia.org/wiki/Normal_distribution for the equation.
double GetNormalDistributionValue(
    const double x, const double mean, const double variance) {

  const double part_1 = 1.0 / std::sqrt(2 * M_PI * variance);

  const double x_mean = x - mean;
  const double x_mean2 = x_mean * x_mean;
  const double exponent = -x_mean2 / (2 * variance);
  const double part_2 = std::exp(exponent);

  return part_1 * part_2;
}

}  // namespace

std::vector<double> GenerateSpectrum(
    const std::vector<PeakDistribution>& peaks, const int num_bands) {

  std::vector<double> spectrum(num_bands);
  double max_value = 0.0;
  for (int band = 0; band < num_bands; ++band) {
    const double normalized_x =
        static_cast<double>(band) / static_cast<double>(num_bands);;
    spectrum[band] = 0.0;
    for (const PeakDistribution& peak : peaks) {
      spectrum[band] += peak.amplitude * GetNormalDistributionValue(
          normalized_x, peak.peak_position, peak.width);
    }
    max_value = std::max(spectrum[band], max_value);
  }
  // Normalize the spectrum between 0 and 1.
  if (max_value > 1.0) {
    for (int band = 0; band < num_bands; ++band) {
      spectrum[band] /= max_value;
    }
  }
  return spectrum;
}

}  // namespace spectrum_generator
}  // namespace hsi_data_generator
