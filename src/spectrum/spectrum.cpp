#include "spectrum/spectrum.h"

#include <QtDebug>

#include <math.h>

#include <algorithm>
#include <cmath>
#include <vector>

namespace hsi_data_generator {
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

void Spectrum::AddPeak(
    const double position, const double amplitude, const double width) {

  // TODO: Check validity of the given values (make sure they're in range).
  PeakDistribution peak;
  peak.position = position;
  peak.amplitude = amplitude;
  peak.width = width;
  spectral_peaks_.push_back(peak);
}

void Spectrum::UpdatePeak(
    const int peak_index,
    const double new_position,
    const double new_amplitude,
    const double new_width) {

  if (peak_index < 0 || peak_index >= spectral_peaks_.size()) {
    qWarning() << "Peak index " << peak_index << " is out of range: "
               << "must be between 0 and " << (spectral_peaks_.size() - 1);
    return;
  }
  spectral_peaks_[peak_index].position = new_position;
  spectral_peaks_[peak_index].amplitude = new_amplitude;
  spectral_peaks_[peak_index].width = new_width;
}

void Spectrum::Reset() {
  spectral_peaks_.clear();
}

std::vector<double> Spectrum::GenerateSpectrum(const int num_bands) const {
  std::vector<double> spectrum(num_bands);
  double max_value = 0.0;
  for (int band = 0; band < num_bands; ++band) {
    const double normalized_x =
        static_cast<double>(band) / static_cast<double>(num_bands);;
    spectrum[band] = 0.0;
    for (const PeakDistribution& peak : spectral_peaks_) {
      spectrum[band] += peak.amplitude * GetNormalDistributionValue(
          normalized_x, peak.position, peak.width);
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

}  // namespace hsi_data_generator
