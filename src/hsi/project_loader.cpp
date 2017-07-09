#include "hsi/project_loader.h"

#include <QColor>
#include <QFile>
#include <QIODevice>
#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "hsi/spectrum.h"
#include "util/util.h"

namespace hsi_data_generator {
namespace {

// Save file constants:
static const QString kProjectFileHeader =
    "Project File Exported by HSIDataGenerator";

static const QString kSpectralDictionaryTag = "spectral_dictionary";
static const QString kSpectrumTag = "spectrum";
static const QString kSpectrumPeaksTag = "peaks";
static const QString kSpectrumNameTag = "name";
static const QString kSpectrumColorTag = "color";
static const QString kPeakTag = "peak";
static const QString kPeakPositionTag = "position";
static const QString kPeakAmplitudeTag = "amplitude";
static const QString kPeakWidthTag = "width";
static const QString kNumBandsTag = "num_bands";

// Error messages:
static const QString kGenericErrorMessage =
    "File read/write error.";
static const QString kFileNotOpenWriteErrorMessage =
    "Could not open file \"" + util::kTextSubPlaceholder + "\" for writing.";
static const QString kFileNotOpenReadErrorMessage =
    "Could not open file \"" + util::kTextSubPlaceholder + "\" for reading.";

}  // namespace

bool ProjectLoader::SaveProjectToFile(const QString& file_name) const {
  QFile project_file(file_name);
  if (!project_file.open(QIODevice::WriteOnly)) {
    error_message_ = util::ReplaceTextSubPlaceholder(
        kFileNotOpenWriteErrorMessage, file_name);
    return false;
  }
  QXmlStreamWriter xml_writer(&project_file);
  xml_writer.setAutoFormatting(true);
  xml_writer.writeStartDocument();
  xml_writer.writeComment(kProjectFileHeader);

  // <spectral_dictionary>
  xml_writer.writeStartElement(kSpectralDictionaryTag);
  for (int i = 0; i < spectra_->size(); ++i) {
    const std::shared_ptr<Spectrum> spectrum = spectra_->at(i);
    xml_writer.writeStartElement(kSpectrumTag);  // <spectrum>
    xml_writer.writeStartElement(kSpectrumPeaksTag);  // <peaks>
    for (const PeakDistribution& peak : spectrum->GetPeaks()) {
      xml_writer.writeStartElement(kPeakTag);  // <peak>
      xml_writer.writeTextElement(  // <position> </position>
          kPeakPositionTag, QString::number(peak.position));
      xml_writer.writeTextElement(  // <amplitude> </amplitude>
          kPeakAmplitudeTag, QString::number(peak.amplitude));
      // <width> </width>
      xml_writer.writeTextElement(kPeakWidthTag, QString::number(peak.width));
      xml_writer.writeEndElement();  // </peak>
    }
    xml_writer.writeEndElement();  // </peaks>
    // <name> </name>
    xml_writer.writeTextElement(kSpectrumNameTag, spectrum->GetName());
    // <color> </color>
    xml_writer.writeTextElement(kSpectrumColorTag, spectrum->GetColor().name());
    xml_writer.writeEndElement();  // </spectrum>
  }
  xml_writer.writeTextElement(kNumBandsTag, QString::number(*num_bands_));
  xml_writer.writeEndElement();  // </spectral_dictionary>

//  // TODO: Save the layout.
//  // Save the layout information in the following format:
//  // Layout:
//  // layout_width = 100
//  // layout_height = 100
//  // layout {
//  //   row 1
//  //   0 0 0 0 0 1 1 1 2 2 2 1 1 1 0 0 1 ... 0 1 0
//  //   1 2 2 2 2 2 1 1 2 3 3 4 5 5 5 5 ....
//  //   ...
//  //   row 100
//  // }

  project_file.close();
  return true;
}

bool ProjectLoader::LoadProjectFromFile(const QString& file_name) {
  QFile project_file(file_name);
  if (!project_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    error_message_ = util::ReplaceTextSubPlaceholder(
        kFileNotOpenReadErrorMessage, file_name);
    return false;
  }
  QXmlStreamReader xml_reader(&project_file);
  if (xml_reader.readNextStartElement()) {
    if (xml_reader.name() == kSpectralDictionaryTag) {  // <spectral_dictionary>
      spectra_->clear();
      while (xml_reader.readNextStartElement()) {
        if (xml_reader.name() == kSpectrumTag) {  // <spectrum>
          std::shared_ptr<Spectrum> spectrum(new Spectrum());
          while (xml_reader.readNextStartElement()) {
            if (xml_reader.name() == kSpectrumPeaksTag) {  // <peaks>
              while (xml_reader.readNextStartElement()) {
                if (xml_reader.name() == kPeakTag) {  // <peak>
                  PeakDistribution peak;
                  while (xml_reader.readNextStartElement()) {
                    if (xml_reader.name() == kPeakPositionTag) {
                      const QString peak_position_text =  // <position>
                          xml_reader.readElementText();
                      peak.position = peak_position_text.toDouble();
                    } else if (xml_reader.name() == kPeakAmplitudeTag) {
                      const QString peak_amplitude_text =  // <amplitude>
                          xml_reader.readElementText();
                      peak.amplitude = peak_amplitude_text.toDouble();
                    } else if (xml_reader.name() == kPeakWidthTag) {
                      const QString peak_width_text =  // <width>
                          xml_reader.readElementText();
                      peak.width = peak_width_text.toDouble();
                    } else {
                      xml_reader.skipCurrentElement();  // Unknown tag.
                    }
                  }
                  // </peak>
                  spectrum->AddPeak(peak.position, peak.amplitude, peak.width);
                } else {
                  xml_reader.skipCurrentElement();  // Unknown tag.
                }
              }
              // </peaks>
            } else if (xml_reader.name() == kSpectrumNameTag) {  // <name>
              spectrum->SetName(xml_reader.readElementText());
            } else if (xml_reader.name() == kSpectrumColorTag) {  // <color>
              const QString color_string = xml_reader.readElementText();
              const QColor color(color_string);
              spectrum->SetColor(color);
            }
          }
          spectra_->push_back(spectrum);
          // </spectrum>
        } else if (xml_reader.name() == kNumBandsTag) {  // <num_bands>
          const QString num_bands_string = xml_reader.readElementText();
          *num_bands_ = num_bands_string.toInt();
        } else {
          xml_reader.skipCurrentElement();  // Unknown tag.
        }
      }
    }
    // </spectral_dictionary>
  }
  project_file.close();
  return true;
}

QString ProjectLoader::GetErrorMessage() const {
  if (error_message_.isEmpty()) {
    return kGenericErrorMessage;
  }
  return error_message_;
}

}  // namespace hsi_data_generator
