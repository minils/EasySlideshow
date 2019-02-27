#ifndef SLIDESHOWIMAGE_H
#define SLIDESHOWIMAGE_H

#include <QObject>
#include <QFileInfo>
#include <QSize>
#include <QImageReader>
#include <QDir>
#include <QDateTime>

#include <QImageReader>
#include <QImageWriter>

#include <libexif/exif-data.h>
#include <libexif/exif-loader.h>

#include "globals.h"
#include "settingsmanager.h"

class SlideshowImage
{
private:
    int _orientation;
    QString _path;

    void exif_transform(const QString path, int angle) const;

public:
    explicit SlideshowImage(QString path = "");

    // Getter
    const QString path() const;
    const QString filename() const;
    const QString directory() const;
    long long filesize() const;
    const QSize resolution() const;
    int orientation() const;
    QDateTime creationTime() const;

    // Setter
    void rotateLeft();
    void rotateRight();
    void rotate(int angle);
    void setPath(QString path);

signals:

public slots:
};

#endif // SLIDESHOWIMAGE_H
