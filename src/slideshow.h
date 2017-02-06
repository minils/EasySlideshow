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

class SlideShow : public QObject
{
    Q_OBJECT
public:
    explicit SlideShow(QList<QDir> *dirs, unsigned int time, QObject *parent = 0);
    void setDirs(QList<QDir> *dirs);
    void setSpeed(unsigned int speed);
    bool paused(void);
    bool scanningIsActive();

private:
    QList<QDir> *_dirs;
    bool _dirs_valid;
    unsigned int _speed;

    unsigned int _last;                     // index of the last image in _images
    QStringList *_images;

    bool _pause;
    QTimer *timer;
    PathScanner *pathScanner;

    QString _current_path;                  // path of the currently displayed image
    unsigned int _current;                  // index of the current image in _previous_images
    QList<unsigned int> _previous_images;   // last shown images
    QHash<unsigned int, int> _images_orientation;

    SettingsManager *_settingsManager;

    void loadImage(unsigned int current);
    volatile bool scanningActive;
    void rotateCurrentImage(int direction);

signals:
    void dirChecked(bool exists, bool readable);
    void showPath(QString path);

    void showImage(const QPixmap *image);
    void timeout(void);
    void communicatePause(void);
    void initStart(void);
    void initStop(void);
    void stopScan(void);

public slots:
    void nextImage(void);
    void pause(void);
    void imageClicked(void);
    void init(void);
    void nextImageClicked(void);
    void previousImageClicked(void);
    void rotateCurrentImageLeft(void);
    void rotateCurrentImageRight(void);
    void initDone(QStringList *images);
    void scanStopped(void);
};

#endif // SLIDESHOW_H
