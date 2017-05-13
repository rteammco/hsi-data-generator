#ifndef SRC_SPECTRUM_SPECTRUM_H_
#define SRC_SPECTRUM_SPECTRUM_H_

#include <QColor>
#include <QString>

#include <vector>

namespace hsi_data_generator {

// The distribution variables for a single spectrum peak. These values encode a
// Gaussian distribution that represents a single peak in the spectrum. All
// values should be normalized between 0 and 1.
struct PeakDistribution {
  // The position is the x-position of the peak. The peak will be centered at
  // this position. It is the mean of the Guassian.
  //
  // Position should be normalized between 0 and 1.
  double position;

  // The amplitude is the "height" of the peak. 0 is nothing, 1 is the maximum
  // height.
  double amplitude;

  // The width is the standard deviation of the Gaussian, and encodes how thin
  // or thick the peak will be. Sharp, instantaneous peaks should have a small
  // width.
  //
  // A peak's width should be smaller than the span of the entire spectrum,
  // which is a normalized width of 1.0.
  double width;
};

class Spectrum {
 public:
  Spectrum();

  // Adds a peak to the spectrum. The definitions of each of the required
  // values are described in the PeakDistribution struct above.
  void AddPeak(
      const double position, const double amplitude, const double width);

  // Resets the spectrum. All peaks will be removed.
  void Reset();

  // Generates the spectrum from the spectral_peaks_ (distributions). The
  // spectral resolution is determined by the given number of bands.
  //
  // All values of the returned spectrum will be normalized between 0 and 1.
  std::vector<double> GenerateSpectrum(const int num_bands) const;

  // Returns the peaks (for reference only).
  const std::vector<PeakDistribution>& GetPeaks() const {
    return spectral_peaks_;
  }

 private:
  // The name and color associated with this spectrum. The "class" refers to
  // the element (endmember) that this spectrum represents, which is
  // application-specific.
  QString spectrum_class_name_;
  QColor spectrum_class_color_;

  // The peaks that define this spectrum. These peaks are a basis that can be
  // used to generate the spectrum at any spectral resolution.
  std::vector<PeakDistribution> spectral_peaks_;
};

}  // namespace hsi_data_generator

#endif  // SRC_SPECTRUM_SPECTRUM_H_
