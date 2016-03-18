#include "slideshow.h"

/**
 * @brief SlideShow::SlideShow
 * @param parent
 * @param dir Directory with photos
 * @param speed How many seconds between photos
 */
SlideShow::SlideShow(QDir *dir, unsigned int speed, QObject *parent) : QObject(parent)
{
    _dir = dir;
    _speed = speed * 1000;
    _last = 0;
    _current = 0;
    _pause = false;
    _dir_valid = false;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(nextImage()));
    timer->setInterval(_speed);
    timer->setSingleShot(false); // timer fires continuously
}

void SlideShow::setDirectory(QDir *dir)
{
    if (dir->absolutePath() != _dir->absolutePath()) {
        _dir = dir;
        init();
    }
}

void SlideShow::setSpeed(unsigned int speed)
{
    _speed = speed * 1000;
    timer->setInterval(_speed);
}

void SlideShow::init(void)
{
    qDebug() << "[SlideShow] Initializing";
    qsrand(QDateTime::currentDateTime().toTime_t ());
    if (_dir == NULL) {
        return;
    }
    emit dirChecked(_dir->exists(), _dir->isReadable());

    if (!_dir->exists() || !_dir->isReadable()) {
        return;
    } else {
        _dir_valid = true;
    }

    // clear images if needed
    _images.clear();
    _last = 0;
    _pause = false;
    timer->stop();
    _previous_images.clear();

    // else find images
    const QStringList image_filter = (QStringList() << "*.jpg" << "*.jpeg" << "*.bmp" << "*.png" << "*.tif" << "*.tiff" << "*.gif"); // TODO: make global
    QDirIterator iter(_dir->absolutePath(), image_filter, QDir::Files, QDirIterator::Subdirectories);
    while (iter.hasNext()) {
        _images.append(iter.next());
    }
    if (_images.size() == 0) {
        qDebug() << "[SlideShow] Did not find any images.";
        return;
    }
    qDebug() << "[SlideShow] Found" << _images.size() << "images.";

    nextImage();
    timer->start();
}

void SlideShow::nextImage(void)
{
    if (!_dir_valid || _images.size() == 0) {
        qDebug() << "[SlideShow] Called nextImage() but _pause =" << _pause << ", _dir_valid =" << _dir_valid;
        return;
    }
    // we are going back in the list and want to show the next image in the list
    if (_previous_images.size() != 0 && _current < (unsigned int) _previous_images.size()-1) {
        ++_current;
        _current_path = _previous_images.at(_current);
    // we are at the end of the list and want to pick a random image
    } else {
      unsigned int random = _last;
      while (_last == random) {
          random = qrand() % _images.size();
      }
      _last = random;
      _current_path = _images.at(random);
      _previous_images.append(_current_path);
      ++_current;
    }
    loadImage(_current_path);
}

void SlideShow::nextImageClicked(void)
{
    // TODO: why pause? config?
    if (!_pause) {
        emit communicatePause();
    }
    nextImage();
}

void SlideShow::previousImageClicked(void)
{
  if (_previous_images.size() < 2 || _current < 1)
      return;
  if (_current == (unsigned int) _previous_images.size()) {
    if (!_pause) {
        emit communicatePause();
    }
    _current = _previous_images.size() - 2; // go one back
  } else {
    --_current;
  }
  QString path = _previous_images.at(_current);
  loadImage(path);
}

void SlideShow::pause(void)
{
    _pause = !_pause;
    if (_pause) {
        timer->stop();
    } else {
        nextImage();
        timer->start();
    }
}

void SlideShow::openCurrentDirectory(void) {
    if (!_dir_valid || _images.size() == 0) {
        return;
    }
    if (!_pause) {
        // TODO: make configurable
        emit communicatePause();
    }
    QString path = QDir::toNativeSeparators(QFileInfo(_current_path).absolutePath());
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
//    FileUtils::showInGraphicalShell(path);
}

void SlideShow::loadImage(QString path)
{
  // TODO: read EXIF data and rotate if necessary
  QPixmap pix(path);
  emit showImage(&pix);
  emit showPath(path);
}

bool SlideShow::paused(void)
{
    return _pause;
}
