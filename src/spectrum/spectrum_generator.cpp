#include "spectrum/spectrum_generator.h"

#include <cmath>  // TODO: Remove when debug code is gone (for sin and abs).
#include <vector>

namespace hsi_data_generator {

void SpectrumGenerator::AddPeak(const PeakDistribution& peak) {
  peaks_.push_back(peak);
}

std::vector<double> SpectrumGenerator::GetNormalizedSpectrum() const {
  std::vector<double> spectrum;

  // TODO: This is temporary, fix!
  for (int i = 0; i < num_bands_; ++i) {
    const double d = static_cast<double>(i) / 10.0;
    const double s =
        (1.0 + (i % 10)) / (static_cast<double>(num_bands_) / 10.0);
    const double value = s * sin(d);
    spectrum.push_back(std::abs(value));
  }

  return spectrum;
}

}  // namespace hsi_data_generator
