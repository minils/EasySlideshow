#ifndef SLIDESHOW_H
#define SLIDESHOW_H

#include <QObject>
#include <QDir>
#include <QtDebug>
#include <QPixmap>
#include <QDirIterator>
#include <QQueue>
#include <QList>
#include <QString>
#include <QDateTime>
#include <QTimer>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QList>
#include <QHash>
#include <QStringList>

#include "settingsmanager.h"
#include "pathscanner.h"
#include "displaylabel.h"
#include "slideshowimage.h"

class SlideShow : public QObject
{
    Q_OBJECT
public:
    explicit SlideShow(DisplayLabel *displayLabel, QList<QDir> *dirs, int time, QObject *parent = nullptr);
    void setDirs(QList<QDir> *dirs);
    void setSpeed(int speed);
    bool paused(void);
    bool scanningIsActive();
    void pauseSlideshow(bool newStatus);

private:
    QList<QDir> *_dirs;
    bool _dirs_valid;
    int _speed;

    int _last;                     // index of the last image in _images
    QStringList *_images;
    QList<SlideshowImage> _slideshowimages;
    int _current;                  // index of the current image in _slideshowimages

    bool _pause;
    QTimer *timer;
    PathScanner *_pathScanner;
    DisplayLabel *_displayLabel;

    //QString _current_path;                  // path of the currently displayed image

    void loadImage(const SlideshowImage* image);
    volatile bool scanningActive;
    void rotateCurrentImage(int direction);

signals:
    void showImage(const SlideshowImage*);
    void displayError(QString msg);
    void timeout(void);
    void communicatePauseStatus(void);
    void initStart(void);
    void initStop(void);
    void stopScan(void);

public slots:
    void nextImage(void);
    void pauseClicked(void);
    void init(void);
    void nextImageClicked(void);
    void previousImageClicked(void);
    void rotateCurrentImageLeft(void);
    void rotateCurrentImageRight(void);
    void initDone(QStringList *images);
    void scanStopped(void);
};

#endif // SLIDESHOW_H
