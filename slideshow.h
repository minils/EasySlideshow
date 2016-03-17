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
    QList<QString> _images;
    unsigned int _speed;
    unsigned int _last;
    unsigned int _current;
    bool _pause;
    bool _dir_valid;
    QString _current_path;
    QTimer *timer;
    QList<QString> _previous_images; // TODO: only save index of _images[]

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
