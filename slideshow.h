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

class SlideShow : public QObject
{
    Q_OBJECT
public:
    explicit SlideShow(QDir *dir, unsigned int time, QObject *parent = 0);
    void setDirectory(QDir *dir);
    void setSpeed(unsigned int speed);
    bool paused(void);

private:
    QDir *_dir;
    bool _dir_valid;
    unsigned int _speed;

    unsigned int _last;                     // index of the last image in _images
    QList<QString> _images;

    bool _pause;
    QTimer *timer;

    QString _current_path;                  // path of the currently displayed image
    unsigned int _current;                  // index of the current image in _previous_images
    QList<unsigned int> _previous_images;   // last shown images

    void loadImage(QString path);

signals:
    void dirChecked(bool exists, bool readable);
    void showPath(QString path);

    void showImage(const QPixmap *image);
    void timeout(void);
    void communicatePause(void);

public slots:
    void nextImage(void);
    void pause(void);
    void openCurrentDirectory(void);
    void init(void);
    void nextImageClicked(void);
    void previousImageClicked(void);
};

#endif // SLIDESHOW_H
