#include "displaylabel.h"

DisplayLabel::DisplayLabel(QWidget *parent) : QLabel(parent)
{
    this->setMinimumSize(1, 1);
    this->setAlignment(Qt::AlignCenter);
}

void DisplayLabel::setImage(const QPixmap *image)
{
    _image = QPixmap(*image);
    resizeEvent(NULL);
}

void DisplayLabel::resizeEvent(QResizeEvent*)
{
    if (_image.isNull())
        return;
    int w = this->width();
    int h = this->height();
    this->setPixmap(_image.scaled(w, h, Qt::KeepAspectRatio));
}

/**
 * @brief DisplayLabel::mouseReleaseEvent Opens the directory of the current image.
 * @param event
 */
void DisplayLabel::mouseReleaseEvent(QMouseEvent*)
{
    // TODO: test for left click and add right click menu
    emit openCurrentDirectory();
}
