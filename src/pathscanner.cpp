#include "pathscanner.h"

PathScanner::PathScanner(QObject *parent)
    : QThread(parent)
{
    
}

void PathScanner::setPaths(QList<QDir> *dirs)
{
  _dirs = dirs;
}

void PathScanner::run() {
  if (_dirs == NULL) {
    return;
  }
  if (!checkDirs(_dirs)) {
    // some dir does not exist or is not readable
    // TODO: propagate this
  }
  _images->clear();
  // else find images
  const QStringList image_filter =
    QStringList() << "*.jpg" << "*.jpeg" << "*.bmp" << "*.png" << "*.tif" << "*.tiff" << "*.gif";
  QListIterator<QDir> dirs(*_dirs);
  while (dirs.hasNext()) {
    QString absPath = dirs.next().absolutePath();
    qDebug() << "[PathScanner] Scanning folder: " << absPath;
    QDirIterator iter(absPath, image_filter, QDir::Files, QDirIterator::Subdirectories);
    while (iter.hasNext()) {
      _images->append(iter.next());
    }
  }
  if (_images->size() == 0) {
    qDebug() << "[PathScanner] Did not find any images.";
    return;
  }
  qDebug() << "[PathScanner] Found" << _images->size() << "images.";
  emit finnishedScan(_images); 
}

bool PathScanner::checkDirs(QList<QDir> *dirs)
{
  QListIterator<QDir> i(*dirs);
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
