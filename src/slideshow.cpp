#include "slideshow.h"

/**
 * @brief SlideShow::SlideShow
 * @param parent
 * @param dir Directory with photos
 * @param speed How many seconds between photos
 */
SlideShow::SlideShow(DisplayLabel *displayLabel, QList<QDir> *dirs, unsigned int speed, QObject *parent) : QObject(parent)
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

void SlideShow::setSpeed(unsigned int speed)
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
  _current = 0;
  _pause = false;
  timer->stop();
  _previous_images.clear();

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

void SlideShow::nextImage(void)
{
    if (_images->size() == 0) {
        return;
    }
    // we are going back in the list and want to show the next image in the list
    if (_previous_images.size() != 0 && _current < (unsigned int) _previous_images.size()) {
        ++_current;
        _current_path = _images->at(_previous_images.at(_current - 1));
    // we are at the end of the list and want to pick a random image
    } else {
      unsigned int random = _last;
      while (_last == random) {
          random = qrand() % _images->size();
      }
      _last = random;
      _current_path = _images->at(random);
      _previous_images.append(random);
      ++_current;
    }
    loadImage(_current);
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
    if (_previous_images.size() < 2 || _current < 2)
        return;

    if (!_pause) {
        emit communicatePause();
    }
    --_current;
    loadImage(_current);
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

void SlideShow::loadImage(unsigned int current)
{
  _current_path = _images->at(_previous_images.at(current-1));
  _displayLabel->displayImage(_current_path, _images_orientation[current]);
  emit showPath(_current_path);
}

bool SlideShow::paused(void)
{
    return _pause;
}

void SlideShow::rotateCurrentImage(int direction)
{
    if (scanningActive || _current == 0)
        return;
    int new_orientation = _images_orientation.take(_current);
    new_orientation = (new_orientation + (90 * direction / abs(direction))) % 360;
    _images_orientation.insert(_current, new_orientation);
    loadImage(_current);
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
