#include "gui/image_layout_view.h"

#include <QColor>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QLayoutItem>
#include <QMessageBox>
#include <QPushButton>
#include <QShowEvent>
#include <QString>
#include <QWidget>

#include <memory>
#include <vector>

#include "gui/image_layout_widget.h"
#include "hsi/image_layout.h"
#include "hsi/spectrum.h"
#include "util/util.h"

namespace hsi_data_generator {
namespace {

static const QString kQtImageLayoutViewStyle =
    "qt_stylesheets/image_layout_view.qss";

// Popup dialog notification text.
static const QString kNoSpectraErrorDialogTitle = "No Spectra Available";
static const QString kNoSpectraErrorDialogMessage =
    "No spectra are currently available. Use the Spectra Editor to make some.";

static const QString kStripeWidthDialogTitle = "Select Stripe Width";
static const QString kStripeWidthDialogSelectionLabel =
    "Stripe width (0 for automatic size):";

static const QString kSquareSizeDialogTitle = "Select Grid Square Size";
static const QString kSquareSizeDialogSelectionLabel =
    "Square size (0 for automatic size):";

static const QString kRandomBlobSizeDialogTitle = "Select Random Blob Size";
static const QString kRandomBlobSizeDialogSelectionLabel = "Blob size:";

// The layout options for the image controlled by the buttons.
enum ImageLayoutType {
  IMAGE_HORIZONTAL_STRIPES_LAYOUT,
  IMAGE_VERTICAL_STRIPES_LAYOUT,
  IMAGE_GRID_LAYOUT,
  IMAGE_RANDOM_LAYOUT
};

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
  case IMAGE_HORIZONTAL_STRIPES_LAYOUT: {
    const int stripe_width = QInputDialog::getInt(
        dialog_parent,
        kStripeWidthDialogTitle,
        kStripeWidthDialogSelectionLabel,
        0,  // default value
        0,  // min value
        image_layout->GetWidth());  // max value
    image_layout->GenerateHorizontalStripesLayout(num_classes, stripe_width);
    break;
  }
  case IMAGE_VERTICAL_STRIPES_LAYOUT: {
    const int stripe_width = QInputDialog::getInt(
        dialog_parent,
        kStripeWidthDialogTitle,
        kStripeWidthDialogSelectionLabel,
        0,  // default value
        0,  // min value
        image_layout->GetHeight());  // max value
    image_layout->GenerateVerticalStripesLayout(num_classes, stripe_width);
    break;
  }
  case IMAGE_GRID_LAYOUT: {
    const int square_size = QInputDialog::getInt(
        dialog_parent,
        kSquareSizeDialogTitle,
        kSquareSizeDialogSelectionLabel,
        0,  // default value
        0,  // min value
        image_layout->GetWidth());  // max value; TODO: should be max(w, h).
    image_layout->GenerateGridLayout(num_classes, square_size);
    break;
  }
  case IMAGE_RANDOM_LAYOUT: {
    const int blob_size = QInputDialog::getInt(
        dialog_parent,
        kRandomBlobSizeDialogTitle,
        kRandomBlobSizeDialogSelectionLabel,
        1,  // default value
        1,  // min value
        image_layout->GetNumPixels());
    image_layout->GenerateRandomLayout(num_classes, blob_size);
    break;
  }
  default:
    break;
  }
  // Render the results using the ImageLayoutWidget.
  std::vector<QColor> class_colors;
  for (const std::shared_ptr<Spectrum> spectrum : spectra) {
    class_colors.push_back(spectrum->GetColor());
  }
  image_layout_widget->Render(class_colors);
}

}  // namespace

ImageLayoutView::ImageLayoutView(
    std::shared_ptr<std::vector<std::shared_ptr<Spectrum>>> spectra,
    std::shared_ptr<ImageLayout> image_layout)
    : spectra_(spectra), image_layout_(image_layout) {

  setStyleSheet(util::GetStylesheetRelativePath(kQtImageLayoutViewStyle));

  QHBoxLayout* layout = new QHBoxLayout();
  layout->setAlignment(Qt::AlignTop);
  setLayout(layout);

  // Left-hand column lists the class names and colors.
  // TODO: Maybe this should be a widget instead?
  class_names_layout_ = new QVBoxLayout();
  layout->addLayout(class_names_layout_);

  // Center column is the image display widget.
  image_layout_widget_ = new ImageLayoutWidget(image_layout_);
  layout->addWidget(image_layout_widget_);

  // Right-hand column is the set of patterns to generate over the image.
  // TODO: Make this interactive, with preview images, etc.
  QVBoxLayout* pattern_list_layout = new QVBoxLayout();

  QPushButton* horizontal_stripes_button =
      new QPushButton("Horizontal Stripes");
  pattern_list_layout->addWidget(horizontal_stripes_button);
  connect(
      horizontal_stripes_button,
      SIGNAL(released()),
      this,
      SLOT(HorizontalStripesButtonPressed()));

  QPushButton* vertical_stripes_button = new QPushButton("Vertical Stripes");
  pattern_list_layout->addWidget(vertical_stripes_button);
  connect(
      vertical_stripes_button,
      SIGNAL(released()),
      this,
      SLOT(VerticalStripesButtonPressed()));

  QPushButton* grid_button = new QPushButton("Grid");
  pattern_list_layout->addWidget(grid_button);
  connect(
      grid_button,
      SIGNAL(released()),
      this,
      SLOT(GridButtonPressed()));

  QPushButton* random_button = new QPushButton("Random");
  pattern_list_layout->addWidget(random_button);
  connect(
      random_button,
      SIGNAL(released()),
      this,
      SLOT(RandomButtonPressed()));

  // TODO: Add these more advanced options.
//  pattern_list_layout->addWidget(new QLabel("Markov"));
//  pattern_list_layout->addWidget(new QLabel("Custom Draw"));

  layout->addLayout(pattern_list_layout);
}

void ImageLayoutView::UpdateGUI() {
  // Remove all existing spectrum name labels before re-inserting them.
  QLayoutItem* item;
  while ((item = class_names_layout_->itemAt(0)) != nullptr) {
    class_names_layout_->removeItem(item);
    delete item->widget();
    delete item;
  }
  // Re-insert all of the updated labels.
  for (int i = 0; i < spectra_->size(); ++i) {
    const QString class_name = spectra_->at(i)->GetName();
    const QColor class_color = spectra_->at(i)->GetColor();
    QLabel* new_label = new QLabel(class_name);
    QPalette label_palette;
    label_palette.setColor(new_label->foregroundRole(), class_color);
    new_label->setPalette(label_palette);
    class_names_layout_->addWidget(new_label);
  }
}

void ImageLayoutView::showEvent(QShowEvent* event) {
  UpdateGUI();
}

void ImageLayoutView::HorizontalStripesButtonPressed() {
  GenerateLayout(
      IMAGE_HORIZONTAL_STRIPES_LAYOUT,
      *spectra_,
      image_layout_,
      image_layout_widget_,
      this);
}

void ImageLayoutView::VerticalStripesButtonPressed() {
  GenerateLayout(
      IMAGE_VERTICAL_STRIPES_LAYOUT,
      *spectra_,
      image_layout_,
      image_layout_widget_,
      this);
}

void ImageLayoutView::GridButtonPressed() {
  GenerateLayout(
      IMAGE_GRID_LAYOUT, *spectra_, image_layout_, image_layout_widget_, this);
}

void ImageLayoutView::RandomButtonPressed() {
  GenerateLayout(
      IMAGE_RANDOM_LAYOUT,
      *spectra_,
      image_layout_,
      image_layout_widget_,
      this);
}

}  // namespace hsi_data_generator
