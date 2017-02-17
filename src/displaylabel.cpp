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
void DisplayLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit imageClicked();
    }
}

void DisplayLabel::clearImage()
{
    _image = QPixmap(1,1);
    _image.fill(_backgroundColor);
    resizeEvent(NULL);
}

void DisplayLabel::setBackgroundColor(QColor backgroundColor)
{
    _backgroundColor = backgroundColor;
}

void DisplayLabel::contextMenuEvent(QContextMenuEvent *event)
{
  QMenu menu(this);
  QAction *openFolderAction = new QAction(tr("Open &folder"), this);
  connect(openFolderAction, &QAction::triggered, this, [this]() {qDebug() << "clicked open folder";});
  menu.addAction(openFolderAction);
  QAction *openImageAction = new QAction(tr("Open &image"), this);
  openImageAction->setStatusTip(tr("Open image in the default viewer"));
  connect(openImageAction, &QAction::triggered, this, [this]() {qDebug() << "clicked open image";});
  menu.addAction(openImageAction);
  QAction *openDetailsAction = new QAction(tr("&Details"), this);
  connect(openDetailsAction, &QAction::triggered, this, [this]() {qDebug() << "clicked open details";});
  menu.addAction(openDetailsAction);
  menu.exec(event->globalPos());
}
