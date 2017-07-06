#include "hsi/project_loader.h"

#include <QFile>
#include <QIODevice>
#include <QString>
#include <QXmlStreamWriter>

#include "hsi/spectrum.h"

namespace hsi_data_generator {
namespace {

// Save file constants:
static const QString kProjectFileHeader =
    "Project File Exported by HSIDataGenerator";

static const QString kNumBandsTag = "num_bands";
static const QString kSpectrumTag = "spectrum";
static const QString kSpectrumNameTag = "name";
static const QString kSpectrumColorTag = "color";
static const QString kSpectrumPeaksTag = "peaks";
static const QString kPeakTag = "peak";
static const QString kPeakPositionTag = "position";
static const QString kPeakAmplitudeTag = "amplitude";
static const QString kPeakWidthTag = "width";

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

  xml_writer.writeTextElement(kNumBandsTag, QString::number(num_bands_));

  for (int i = 0; i < spectra_->size(); ++i) {
    const std::shared_ptr<Spectrum> spectrum = spectra_->at(i);
    xml_writer.writeStartElement(kSpectrumTag);
    xml_writer.writeTextElement(kSpectrumNameTag, spectrum->GetName());
    xml_writer.writeTextElement(kSpectrumColorTag, spectrum->GetColor().name());
    xml_writer.writeStartElement(kSpectrumPeaksTag);
    for (const PeakDistribution& peak : spectrum->GetPeaks()) {
      xml_writer.writeStartElement(kPeakTag);
      xml_writer.writeTextElement(
          kPeakPositionTag, QString::number(peak.position));
      xml_writer.writeTextElement(
          kPeakAmplitudeTag, QString::number(peak.amplitude));
      xml_writer.writeTextElement(kPeakWidthTag, QString::number(peak.width));
      xml_writer.writeEndElement();
    }
    xml_writer.writeEndElement();
    xml_writer.writeEndElement();
  }

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
  return true;
}

QString ProjectLoader::GetErrorMessage() const {
  if (error_message_.isEmpty()) {
    return kGenericErrorMessage;
  }
  return error_message_;
}

}  // namespace hsi_data_generator
