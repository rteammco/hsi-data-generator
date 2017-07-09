// This class defines a Spectrum as a series of peaks. The spectrum can be
// returned at any desired resolution, and is used to display and export the
// spectral classes and their metadata across all of the GUI components.

#ifndef SRC_HSI_SPECTRUM_H_
#define SRC_HSI_SPECTRUM_H_

#include <QColor>
#include <QString>

#include <vector>

namespace hsi_data_generator {

// The distribution variables for a single spectrum peak. These values encode a
// Gaussian distribution that represents a single peak in the spectrum. All
// values should be normalized between 0 and 1.
struct PeakDistribution {
  // Default constructor initializes everything to 0. This is effectively a
  // zero (null) distribution.
  PeakDistribution() : position(0), amplitude(0), width(0) {}

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
  // Default spectrum sets the name to "New Spectrum" and a random color.
  Spectrum();

  // Creates a spectrum with a random color.
  explicit Spectrum(const QString& spectrum_class_name);

  // Set the spectrum's name and representative color. This can be modified
  // later.
  Spectrum(
      const QString& spectrum_class_name,
      const QColor& spectrum_class_color)
    : spectrum_class_name_(spectrum_class_name),
      spectrum_class_color_(spectrum_class_color) {}

  // Copy constructor. Use this to clone existing spectra.
  Spectrum(const Spectrum& other)
      : spectrum_class_name_(other.spectrum_class_name_),
        spectrum_class_color_(other.spectrum_class_color_),
        spectral_peaks_(other.spectral_peaks_) {}

  // Adds a peak to the spectrum. The definitions of each of the required
  // values are described in the PeakDistribution struct above.
  void AddPeak(
      const double position, const double amplitude, const double width);

  // Updates an existing peak at the given index. If the given index is not
  // valid, nothing will happen (but a warning will be displayed).
  //
  // Peak position, amplitude, and width will always be between 0 and 1. If
  // invalid values are given, they will not be updated. Pass in -1 or any
  // invalid value to keep that variable unchanged.
  void UpdatePeak(
      const int peak_index,
      const double new_position,
      const double new_amplitude,
      const double new_width);

  // Deletes the peak at the given index. If the given index is not valid,
  // nothing will happen (but a warning will be displayed).
  void DeletePeak(const int peak_index);

  // Resets the spectrum. All peaks will be removed.
  void Reset();

  // Change the spectrum's class name. This will be the name identifying this
  // spectrum in the GUI.
  void SetName(const QString& spectrum_class_name) {
    spectrum_class_name_ = spectrum_class_name;
  }

  // Change the spectrum's class color. This is the color that represents this
  // spectrum in the GUI.
  void SetColor(const QColor& spectrum_class_color) {
    spectrum_class_color_ = spectrum_class_color;
  }

  // Generates the spectrum from the spectral_peaks_ (distributions). The
  // spectral resolution is determined by the given number of bands.
  //
  // All values of the returned spectrum will be normalized between 0 and 1.
  std::vector<double> GenerateSpectrum(const int num_bands) const;

  // Returns the name of this spectrum.
  QString GetName() const {
    return spectrum_class_name_;
  }

  // Returns the color of this spectrum.
  QColor GetColor() const {
    return spectrum_class_color_;
  }

  // Returns the peaks (for reference only).
  const std::vector<PeakDistribution>& GetPeaks() const {
    return spectral_peaks_;
  }

  int GetNumPeaks() const {
    return spectral_peaks_.size();
  }

  // Returns true if the spectrum is empty (i.e. it currently has no peaks).
  bool IsEmpty() const {
    return spectral_peaks_.empty();
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

#endif  // SRC_HSI_SPECTRUM_H_
