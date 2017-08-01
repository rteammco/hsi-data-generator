#include "gui/image_layout_view.h"

#include <QColor>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QShowEvent>
#include <QSizePolicy>
#include <QString>
#include <QtDebug>
#include <QWidget>

#include <memory>
#include <vector>

#include "gui/image_layout_widget.h"
#include "hsi/image_layout.h"
#include "hsi/spectrum.h"
#include "util/util.h"

namespace hsi_data_generator {
namespace {

// Identifiers for the layout type for the GenerateLayout() method.
enum ImageLayoutType {
  LAYOUT_TYPE_NONE,
  LAYOUT_TYPE_HORIZONTAL_STRIPES,
  LAYOUT_TYPE_VERTICAL_STRIPES,
  LAYOUT_TYPE_GRID,
  LAYOUT_TYPE_RANDOM,
  LAYOUT_TYPE_IMPORTED_IMAGE
};

static const QString kQtImageLayoutViewStyle =
    "qt_stylesheets/image_layout_view.qss";

// Label and button text.
static const QString kWidthInputLabel = "Width:";
static const QString kHeightInputLabel = "Height:";
static const QString kHorizontalStripesLayoutButtonText = "Horizontal Stripes";
static const QString kVerticalStripesLayoutButtonText = "Vertical Stripes";
static const QString kGridLayoutButtonText = "Grid";
static const QString kRandomLayoutButtonText = "Random";
static const QString kImportImageLayoutButtonText = "Import Image";
static const QString kClearLayoutButtonText = "Clear";

// Popup dialog notification text.
static const QString kNoSpectraErrorDialogTitle = "No Spectra Available";
static const QString kNoSpectraErrorDialogMessage =
    "No spectra are currently available. Use the Spectra Editor to make some.";

// Input dialog selection inputs.
constexpr int kLayoutSizeSelectorNumDecimals = 2;  // Input decimal precision.

static const QString kStripeWidthDialogTitle = "Select Stripe Width";
static const QString kStripeWidthDialogSelectionLabel =
    "Stripe width (0 to 1; 0 for automatic size):";

static const QString kSquareSizeDialogTitle = "Select Grid Square Size";
static const QString kSquareSizeDialogSelectionLabel =
    "Square size (0 to 1; 0 for automatic size):";

static const QString kRandomBlobSizeDialogTitle = "Select Random Blob Size";
static const QString kRandomBlobSizeDialogSelectionLabel = "Blob size:";

static const QString kOpenLayoutImageDialogTitle = "Import Layout Image";
static const QString kOpenLayoutImageErrorDialogTitle = "Error Loading Image";
static const QString kOpenLayoutImageErrorMessage =
    "Could not load file " + util::kTextSubPlaceholder + ".";

// Updates the rendered visualization colors in the ImageLayoutWidget. This is
// used when a new layout is generated or if the spectrum colors are changed.
// The new layout display is then rendered.
void UpdateLayoutVisualization(
    const std::vector<std::shared_ptr<Spectrum>>& spectra,
    ImageLayoutWidget* image_layout_widget) {

  std::vector<QColor> class_colors;
  for (const std::shared_ptr<Spectrum> spectrum : spectra) {
    class_colors.push_back(spectrum->GetColor());
  }
  image_layout_widget->SetClassColors(class_colors);
  image_layout_widget->Render();
}

// The layout options for the image controlled by the buttons.
void GenerateLayout(
    const ImageLayoutType layout_type,
    const std::vector<std::shared_ptr<Spectrum>>& spectra,
    std::shared_ptr<ImageLayout> image_layout,
    ImageLayoutWidget* image_layout_widget,
    QWidget* dialog_parent) {

  // Generate the appropriate layout using the ImageLayout object.
  const int num_classes = spectra.size();
  if (num_classes == 0) {
    QMessageBox::critical(
        dialog_parent,
        kNoSpectraErrorDialogTitle,
        kNoSpectraErrorDialogMessage);
    return;
  }
  switch (layout_type) {
  case LAYOUT_TYPE_HORIZONTAL_STRIPES: {
    bool ok_pressed;
    const double stripe_width = QInputDialog::getDouble(
        dialog_parent,
        kStripeWidthDialogTitle,
        kStripeWidthDialogSelectionLabel,
        0.0,  // default value
        0.0,  // min value
        1.0,  // max value
        kLayoutSizeSelectorNumDecimals,
        &ok_pressed);
    if (!ok_pressed) {
      return;
    }
    image_layout->GenerateHorizontalStripesLayout(num_classes, stripe_width);
    break;
  }
  case LAYOUT_TYPE_VERTICAL_STRIPES: {
    bool ok_pressed;
    const double stripe_width = QInputDialog::getDouble(
        dialog_parent,
        kStripeWidthDialogTitle,
        kStripeWidthDialogSelectionLabel,
        0.0,  // default value
        0.0,  // min value
        1.0,  // max value
        kLayoutSizeSelectorNumDecimals,
        &ok_pressed);
    if (!ok_pressed) {
      return;
    }
    image_layout->GenerateVerticalStripesLayout(num_classes, stripe_width);
    break;
  }
  case LAYOUT_TYPE_GRID: {
    bool ok_pressed;
    const double square_size = QInputDialog::getDouble(
        dialog_parent,
        kSquareSizeDialogTitle,
        kSquareSizeDialogSelectionLabel,
        0.0,  // default value
        0.0,  // min value
        1.0,  // max value
        kLayoutSizeSelectorNumDecimals,
        &ok_pressed);
    if (!ok_pressed) {
      return;
    }
    image_layout->GenerateGridLayout(num_classes, square_size);
    break;
  }
  case LAYOUT_TYPE_RANDOM: {
    bool ok_pressed;
    const int blob_size = QInputDialog::getInt(
        dialog_parent,
        kRandomBlobSizeDialogTitle,
        kRandomBlobSizeDialogSelectionLabel,
        1,  // default value
        1,  // min value
        image_layout->GetNumPixels(),
        1,  // slider step size
        &ok_pressed);
    if (!ok_pressed) {
      return;
    }
    image_layout->GenerateRandomLayout(num_classes, blob_size);
    break;
  }
  case LAYOUT_TYPE_IMPORTED_IMAGE: {
    const QString image_file_name = QFileDialog::getOpenFileName(
        dialog_parent,
        kOpenLayoutImageDialogTitle,         // Dialog save caption.
        util::GetRootCodeDirectory(),        // Default directory.
        "Image Files (*.png *.jpg *.bmp)");  // File filter
    if (image_file_name.isEmpty()) {
      return;
    }
    QImage layout_image;
    if (!layout_image.load(image_file_name)) {
      const QString error_message = util::ReplaceTextSubPlaceholder(
          kOpenLayoutImageErrorMessage, image_file_name);
      QMessageBox::critical(
          dialog_parent,
          kOpenLayoutImageErrorDialogTitle,
          error_message);
      return;
    }
    image_layout->GenerateLayoutFromImage(num_classes, layout_image);
  }
  default:
    break;
  }
  // Render the results using the ImageLayoutWidget.
  UpdateLayoutVisualization(spectra, image_layout_widget);
}

}  // namespace

ImageLayoutView::ImageLayoutView(
    std::shared_ptr<std::vector<std::shared_ptr<Spectrum>>> spectra,
    std::shared_ptr<ImageLayout> image_layout)
    : spectra_(spectra), image_layout_(image_layout) {

  setStyleSheet(util::GetStylesheetRelativePath(kQtImageLayoutViewStyle));

  QHBoxLayout* layout = new QHBoxLayout();
  layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  setLayout(layout);

  // Left-hand column lists the class names and colors.
  class_names_list_ = new QListWidget();
  class_names_list_->viewport()->setAutoFillBackground(false);
  layout->addWidget(class_names_list_);
  connect(
      class_names_list_,
      SIGNAL(itemClicked(QListWidgetItem*)),
      this,
      SLOT(ClassLabelSelected(QListWidgetItem*)));

  // Center column is the image display widget.
  QVBoxLayout* center_layout = new QVBoxLayout();
  image_layout_widget_ = new ImageLayoutWidget(image_layout_);
  image_layout_widget_->setSizePolicy(
      QSizePolicy::Expanding, QSizePolicy::Preferred);
  center_layout->addWidget(image_layout_widget_);

  // Add the width and height bars as well.
  width_input_ = new QLineEdit(QString::number(image_layout_->GetWidth()));
  QHBoxLayout* width_input_layout = new QHBoxLayout();
  width_input_layout->addStretch();  // Pad left to center widgets.
  width_input_layout->addWidget(new QLabel(kWidthInputLabel));
  width_input_layout->addWidget(width_input_);
  width_input_layout->addStretch();  // Pad right to center widgets.
  center_layout->addLayout(width_input_layout);
  center_layout->setAlignment(width_input_layout, Qt::AlignCenter);
  connect(
      width_input_, SIGNAL(returnPressed()), this, SLOT(SizeInputChanged()));

  height_input_ = new QLineEdit(QString::number(image_layout_->GetHeight()));
  QHBoxLayout* height_input_layout = new QHBoxLayout();
  height_input_layout->addStretch();  // Pad left to center widgets.
  height_input_layout->addWidget(new QLabel(kHeightInputLabel));
  height_input_layout->addWidget(height_input_);
  height_input_layout->addStretch();  // Pad right to center widgets.
  center_layout->addLayout(height_input_layout);
  center_layout->setAlignment(height_input_layout, Qt::AlignCenter);
  connect(
      height_input_, SIGNAL(returnPressed()), this, SLOT(SizeInputChanged()));

  center_layout->addStretch();
  layout->addLayout(center_layout);

  // Right-hand column is the set of patterns to generate over the image.
  // TODO: Make this interactive, with preview images, etc.
  QVBoxLayout* pattern_list_layout = new QVBoxLayout();
  pattern_list_layout->setAlignment(Qt::AlignTop);

  QPushButton* horizontal_stripes_button =
      new QPushButton(kHorizontalStripesLayoutButtonText);
  pattern_list_layout->addWidget(horizontal_stripes_button);
  connect(
      horizontal_stripes_button,
      SIGNAL(released()),
      this,
      SLOT(HorizontalStripesButtonPressed()));

  QPushButton* vertical_stripes_button =
      new QPushButton(kVerticalStripesLayoutButtonText);
  pattern_list_layout->addWidget(vertical_stripes_button);
  connect(
      vertical_stripes_button,
      SIGNAL(released()),
      this,
      SLOT(VerticalStripesButtonPressed()));

  QPushButton* grid_button = new QPushButton(kGridLayoutButtonText);
  pattern_list_layout->addWidget(grid_button);
  connect(
      grid_button,
      SIGNAL(released()),
      this,
      SLOT(GridButtonPressed()));

  QPushButton* random_button = new QPushButton(kRandomLayoutButtonText);
  random_button->setEnabled(false);
  pattern_list_layout->addWidget(random_button);
  connect(
      random_button,
      SIGNAL(released()),
      this,
      SLOT(RandomButtonPressed()));

  QPushButton* import_image_button =
      new QPushButton(kImportImageLayoutButtonText);
  import_image_button->setEnabled(false);
  pattern_list_layout->addWidget(import_image_button);
  connect(
      import_image_button,
      SIGNAL(released()),
      this,
      SLOT(ImportImageButtonPressed()));

  QPushButton* clear_button = new QPushButton(kClearLayoutButtonText);
  pattern_list_layout->addWidget(clear_button);
  connect(clear_button, SIGNAL(released()), this, SLOT(ClearButtonPressed()));

  // TODO: Add these more advanced options.
//  pattern_list_layout->addWidget(new QLabel("Markov"));
//  pattern_list_layout->addWidget(new QLabel("Custom Draw"));

  layout->addLayout(pattern_list_layout);
}

void ImageLayoutView::UpdateGUI() {
  // Remove all existing spectrum name labels before re-inserting them.
  class_names_list_->clear();
  for (int i = 0; i < spectra_->size(); ++i) {
    const QString class_name = spectra_->at(i)->GetName();
    const QColor class_color = spectra_->at(i)->GetColor();
    class_names_list_->addItem(class_name);
    class_names_list_->item(i)->setForeground(class_color);
  }
  UpdateLayoutVisualization(*spectra_, image_layout_widget_);
}

void ImageLayoutView::showEvent(QShowEvent* event) {
  UpdateGUI();
}

void ImageLayoutView::HorizontalStripesButtonPressed() {
  GenerateLayout(
      LAYOUT_TYPE_HORIZONTAL_STRIPES,
      *spectra_,
      image_layout_,
      image_layout_widget_,
      this);
}

void ImageLayoutView::VerticalStripesButtonPressed() {
  GenerateLayout(
      LAYOUT_TYPE_VERTICAL_STRIPES,
      *spectra_,
      image_layout_,
      image_layout_widget_,
      this);
}

void ImageLayoutView::GridButtonPressed() {
  GenerateLayout(
      LAYOUT_TYPE_GRID, *spectra_, image_layout_, image_layout_widget_, this);
}

void ImageLayoutView::RandomButtonPressed() {
  GenerateLayout(
      LAYOUT_TYPE_RANDOM,
      *spectra_,
      image_layout_,
      image_layout_widget_,
      this);
}

void ImageLayoutView::ImportImageButtonPressed() {
  GenerateLayout(
      LAYOUT_TYPE_IMPORTED_IMAGE,
      *spectra_,
      image_layout_,
      image_layout_widget_,
      this);
}

void ImageLayoutView::ClearButtonPressed() {
  image_layout_->ResetLayout();
  image_layout_widget_->Render();
}

void ImageLayoutView::SizeInputChanged() {
  if (width_input_ == nullptr || height_input_ == nullptr) {
    qCritical() << "Image size input(s) not defined. Cannot change size.";
    return;
  }
  const QString width_string = width_input_->text();
  const int new_width = width_string.toInt();
  const QString height_string = height_input_->text();
  const int new_height = height_string.toInt();
  if (new_width > 0 && new_height > 0) {
    image_layout_->SetImageSize(new_width, new_height);
    image_layout_widget_->Render();
  }
  // Display the valid value as it is rendered (in case this is different from
  // the user's input):
  width_input_->setText(QString::number(image_layout_->GetWidth()));
  height_input_->setText(QString::number(image_layout_->GetHeight()));
}

void ImageLayoutView::ClassLabelSelected(
    QListWidgetItem* selected_item) {

  const int class_index = class_names_list_->row(selected_item);
  image_layout_widget_->SetUserSelectedClass(class_index);
}

}  // namespace hsi_data_generator
