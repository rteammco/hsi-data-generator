#include "hsi/project_loader.h"

#include <QFile>
#include <QIODevice>
#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QtDebug>

#include "hsi/spectrum.h"

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
static const QString kSubstitutePlaceholder = "%";

static const QString kGenericErrorMessage =
    "File read/write error.";
static const QString kFileNotOpenWriteErrorMessage =
    "Could not open file \"" + kSubstitutePlaceholder + "\" for writing.";
static const QString kFileNotOpenReadErrorMessage =
    "Could not open file \"" + kSubstitutePlaceholder + "\" for reading.";

}  // namespace

bool ProjectLoader::SaveProjectToFile(const QString& file_name) const {
  QFile project_file(file_name);
  if (!project_file.open(QIODevice::WriteOnly)) {
    error_message_ = kFileNotOpenWriteErrorMessage;
    error_message_.replace(kSubstitutePlaceholder, file_name);
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
  xml_writer.writeTextElement(kNumBandsTag, QString::number(num_bands_));
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
    error_message_ = kFileNotOpenReadErrorMessage;
    error_message_.replace(kSubstitutePlaceholder, file_name);
    return false;
  }
  QXmlStreamReader xml_reader(&project_file);
  if (xml_reader.readNextStartElement()) {
    if (xml_reader.name() == kSpectralDictionaryTag) {  // <spectral_dictionary>
      while (xml_reader.readNextStartElement()) {
        if (xml_reader.name() == kSpectrumTag) {  // <spectrum>
          while (xml_reader.readNextStartElement()) {
            if (xml_reader.name() == kSpectrumPeaksTag) {  // <peaks>
              while (xml_reader.readNextStartElement()) {
                if (xml_reader.name() == kPeakTag) {  // <peak>
                  while (xml_reader.readNextStartElement()) {
                    // <position>, <amplitude>, <width>
                    if (xml_reader.name() == kPeakPositionTag) {
                      qInfo() << "POSITION:" << xml_reader.readElementText();
                    } else if (xml_reader.name() == kPeakAmplitudeTag) {
                      qInfo() << "AMPLITUDE:" << xml_reader.readElementText();
                    } else if (xml_reader.name() == kPeakWidthTag) {
                      qInfo() << "WIDTH:" << xml_reader.readElementText();
                    } else {
                      xml_reader.skipCurrentElement();  // Unknown tag.
                    }
                  }
                  // </peak>
                } else {
                  xml_reader.skipCurrentElement();  // Unknown tag.
                }
              }
              // </peaks>
            } else if (xml_reader.name() == kSpectrumNameTag) {  // <name>
              qInfo() << "NAME:" << xml_reader.readElementText();
            } else if (xml_reader.name() == kSpectrumColorTag) {  // <color>
              qInfo() << "COLOR:" << xml_reader.readElementText();
            }
          }
          // </spectrum>
        } else if (xml_reader.name() == kNumBandsTag) {  // <num_bands>
          qInfo() << "NUM BANDS = " << xml_reader.readElementText();
        } else {
          xml_reader.skipCurrentElement();  // Unknown tag.
        }
      }
    }
    // </spectral_dictionary>
  }
  project_file.close();

  return false;
}

QString ProjectLoader::GetErrorMessage() const {
  if (error_message_.isEmpty()) {
    return kGenericErrorMessage;
  }
  return error_message_;
}

}  // namespace hsi_data_generator
