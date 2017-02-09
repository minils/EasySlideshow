#include "pathscanner.h"

PathScanner::PathScanner(QObject *parent)
    : QThread(parent)
{
    _images = new QStringList();
    stop_requested = false;
}

void PathScanner::setPaths(QList<QDir> *dirs)
{
  _dirs = dirs;
}

void PathScanner::run() {
  qDebug() << "[PathScanner] Scan started";
  if (_dirs == NULL || _dirs->empty()) {
    return;
  }
  if (!checkDirs()) {
    // some dir does not exist or is not readable
    // TODO: propagate this
  }
  _images->clear();
  // else find images
  const QStringList image_filter =
    QStringList() << "*.jpg" << "*.jpeg" << "*.bmp" << "*.png" << "*.tif" << "*.tiff" << "*.gif";
  QListIterator<QDir> dirs(*_dirs);
  while (dirs.hasNext() && !stop_requested) {
    QString absPath = dirs.next().absolutePath();
    qDebug() << "[PathScanner] Scanning folder: " << absPath;
    QDirIterator iter(absPath, image_filter, QDir::Files, QDirIterator::Subdirectories);
    while (iter.hasNext() && !stop_requested) {
      _images->append(iter.next());
    }
  }
  if (_images->size() == 0) {
    qDebug() << "[PathScanner] Did not find any images.";
    emit finished(_images);
  }
  qDebug() << "[PathScanner] Found" << _images->size() << "images.";
  if (stop_requested) {
      qDebug() << "Finally stopped";
      stop_requested = false;
      emit stopped();
  } else {
    emit finished(_images);
  }
}

bool PathScanner::checkDirs(void)
{
  QListIterator<QDir> i(*_dirs);
  while (i.hasNext()) {
    QDir dir = i.next();
    if (! dir.exists()) {
      return false;
    }
    if (! dir.isReadable()) {
      return false;
    }
  }
  return true;
}

void PathScanner::request_stop()
{
    qDebug() << "stop requested";
    stop_requested = true;
}
