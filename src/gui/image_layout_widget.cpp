#include "gui/image_layout_widget.h"

#include <QString>

#include "util/util.h"

namespace hsi_data_generator {
namespace {

static const QString kQtImageLayoutStyle =
    "qt_stylesheets/image_layout_widget.qss";

}  // namespace

ImageLayoutWidget::ImageLayoutWidget() {
  setStyleSheet(util::GetStylesheetRelativePath(kQtImageLayoutStyle));
}

}  // namespace hsi_data_generator
