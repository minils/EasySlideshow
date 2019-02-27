#include "slideshow.h"

/**
 * @brief SlideShow::SlideShow
 * @param parent
 * @param dir Directory with photos
 * @param speed How many seconds between photos
 */
SlideShow::SlideShow(DisplayLabel *displayLabel, QList<QDir> *dirs, int speed, QObject *parent) : QObject(parent)
{
    _dirs = dirs;
    _speed = speed * 1000;
    _last = 0;
    _current = 0;
    _pause = false;
    _dirs_valid = false;
    _displayLabel = displayLabel;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(nextImage()));
    timer->setInterval(_speed);
    timer->setSingleShot(false); // timer fires continuously
    qsrand(QDateTime::currentDateTime().toTime_t ());
    _pathScanner = new PathScanner();
    connect(_pathScanner, SIGNAL(finished(QStringList*)), this, SLOT(initDone(QStringList*)));
    connect(this, SIGNAL(stopScan()), _pathScanner, SLOT(request_stop()));
    connect(_pathScanner, SIGNAL(stopped()), this, SLOT(scanStopped()));
    scanningActive = false;
}

void SlideShow::setDirs(QList<QDir> *dirs)
{
    // check if list changed:
    bool equal = true;
    if (_dirs->count() == dirs->count()) {
        // size is the same -> could be equal
        QListIterator<QDir> dirs1(*_dirs);
        QListIterator<QDir> dirs2(*dirs);
        while (dirs1.hasNext()) {
            if (dirs1.next().absolutePath() != dirs2.next().absolutePath()) {
                // not the same elements -> unequal
                equal = false;
                break;
            }
        }
    } else {
        equal = false;
    }

    if (!equal) {
        _dirs = dirs;
        init();
    }
}

void SlideShow::setSpeed(int speed)
{
    _speed = speed * 1000;
    timer->setInterval(_speed);
}

void SlideShow::init(void)
{
  if (scanningActive) {
    emit stopScan();
    return;
  }
  emit initStart();
  scanningActive = true;
  qDebug() << "[SlideShow] Initializing";

  _last = 0;
  _current = -1;
  _pause = false;
  timer->stop();
  _slideshowimages.clear();

  _pathScanner->setPaths(_dirs);
  _pathScanner->start();
}

void SlideShow::scanStopped(void)
{
    scanningActive = false;
    init();
}

void SlideShow::initDone(QStringList *images)
{
  if (images->empty()) {
      emit initStop();
      emit displayError(tr("Did not find any images"));
  }
  _images = images;
  nextImage();
  emit initStop();
  scanningActive = false;
  timer->start();
}

void SlideShow::nextImage(void) {
    if (_images->size() == 0) {
        return;
    }
    const SlideshowImage* next = nullptr;
    if (_slideshowimages.size() != 0 && _current < _slideshowimages.size()-1) {
        ++_current;
        next = &(_slideshowimages.at(_current));
    } else {
        // we are at the end of the list and want to pick a random image
        int random = _last;
        while (_last == random && _images->size()) {
            random = qrand() % _images->size();
        }
        _last = random;
        next = new SlideshowImage(_images->at(random));
        _slideshowimages.append(*next);
        ++_current;
    }
    loadImage(next);
}

void SlideShow::nextImageClicked(void)
{
    timer->stop();
    nextImage();
    if (!_pause) {
        timer->start();
    }
}

void SlideShow::previousImageClicked(void)
{
    if (_slideshowimages.size() < 2 || _current < 1)
        return;

    if (!_pause) {
        pauseSlideshow(true);
    }
    --_current;
    loadImage(&_slideshowimages[_current]);
}

void SlideShow::pauseClicked(void)
{
  pauseSlideshow(!_pause);
}

void SlideShow::pauseSlideshow(bool newStatus)
{
  _pause = newStatus;
  if (_pause) {
      timer->stop();
  } else {
      timer->start();
  }
  qWarning() << "New pause status is: " << newStatus;
  emit communicatePauseStatus();
}

void SlideShow::loadImage(const SlideshowImage* image) {
    qWarning() << "_current = " << _current;
    if (image == nullptr) {
        qWarning() << "loadImage() image is nullptr";
        return;
    }
    _displayLabel->setImage(image);
    _displayLabel->displayImage();
    emit showImage(image);
}

bool SlideShow::paused(void)
{
    return _pause;
}

void SlideShow::rotateCurrentImage(int direction) {
    _slideshowimages[_current].rotate((90 * direction / abs(direction) + 360) % 360);
    loadImage(&_slideshowimages[_current]);
}

void SlideShow::rotateCurrentImageLeft()
{
    rotateCurrentImage(-1);
}

void SlideShow::rotateCurrentImageRight()
{
    rotateCurrentImage(1);
}

bool SlideShow::scanningIsActive()
{
    return scanningActive;
}
