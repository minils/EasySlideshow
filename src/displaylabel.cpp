#include "displaylabel.h"

DisplayLabel::DisplayLabel(QWidget *parent) : QLabel(parent)
{
    this->setMinimumSize(1, 1);
    this->setAlignment(Qt::AlignCenter);
    _path = "";
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
        openFolder(false);
    }
}

void DisplayLabel::clearImage()
{
    _image = QPixmap(1,1);
    _image.fill(_backgroundColor);
    resizeEvent(NULL);
}

void DisplayLabel::displayImage(QString path, int degree)
{
    _path = path;
    QImageReader reader(path);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {
      clearImage();
      return;
    }
    _image = QPixmap(QPixmap::fromImage(newImage));
    if (degree != 0)
        _image = _image.transformed(QTransform().rotate(degree));
    resizeEvent(NULL);
}

void DisplayLabel::setBackgroundColor(QColor backgroundColor)
{
    _backgroundColor = backgroundColor;
}

void DisplayLabel::contextMenuEvent(QContextMenuEvent *event)
{
  if (_path.isEmpty())
      return;
  emit rightMouseSucces(true);
  QMenu menu(this);
  QAction *openFolderAction = new QAction(tr("Open &folder"), this);
  connect(openFolderAction, &QAction::triggered, this, [this]() {this->openFolder(true);});
  menu.addAction(openFolderAction);
  QAction *openImageAction = new QAction(tr("Open &image"), this);
  openImageAction->setStatusTip(tr("Open image in the default viewer"));
  connect(openImageAction, &QAction::triggered, this, [this]() {this->openImage();});
  menu.addAction(openImageAction);
  QAction *openDetailsAction = new QAction(tr("&Details"), this);
  //connect(openDetailsAction, &QAction::triggered, this, [this]() {qDebug() << "clicked open details";});
  menu.addAction(openDetailsAction);
  QAction *result = menu.exec(event->globalPos());
  if (!result) {
    // this starts the slideshow if no item was selected
    //emit rightMouseSucces(false);
    // it is not influenced by the fact that the slideshow was paused before the right click
    return;
  } else if (result == openDetailsAction) {
    emit openDetails(_path);
    DetailsDialog *detailsDialog = new DetailsDialog();
    detailsDialog->show();
    detailsDialog->setFile(_path);
  }
}

void DisplayLabel::openFolder(bool ignoreSettings)
{
    QString clickSetting = SettingsManager::readSetting(SETTING_ON_CLICK_ACTION).toString();
    if (!ignoreSettings && (_path.isEmpty() || clickSetting != SETTING_ON_CLICK_ACTION_OPEN_FOLDER)) {
        return;
    }
    QString path = QDir::toNativeSeparators(QFileInfo(_path).absolutePath());
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void DisplayLabel::openImage()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(_path));
}

