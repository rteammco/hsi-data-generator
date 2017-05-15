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

  // We use UpdatePeak, which will automatically ensure the peak's values are
  // within valid ranges.
  spectral_peaks_.push_back(PeakDistribution());
  UpdatePeak(spectral_peaks_.size() - 1, position, amplitude, width);
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
  if (new_position >= 0.0 && new_position <= 1.0) {
    spectral_peaks_[peak_index].position = new_position;
  }
  if (new_amplitude >= 0.0 && new_amplitude <= 1.0) {
    spectral_peaks_[peak_index].amplitude = new_amplitude;
  }
  if (new_width >= 0.0 && new_width <= 1.0) {
    spectral_peaks_[peak_index].width = new_width;
  }
}

void Spectrum::DeletePeak(const int peak_index) {
  if (peak_index < 0 || peak_index >= spectral_peaks_.size()) {
    qWarning() << "Peak index " << peak_index << " is out of range: "
               << "must be between 0 and " << (spectral_peaks_.size() - 1);
    return;
  }
  spectral_peaks_.erase(spectral_peaks_.begin() + peak_index);
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
      const double peak_value =
          GetNormalDistributionValue(normalized_x, peak.position, peak.width);
      const double peak_max_value =
          GetNormalDistributionValue(peak.position, peak.position, peak.width);
      const double relative_peak_value =
          std::min(1.0, (peak_value / peak_max_value));
      spectrum[band] += peak.amplitude * relative_peak_value;
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
