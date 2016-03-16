#ifndef DISPLAYLABEL_H
#define DISPLAYLABEL_H

#include <QLabel>
#include <QPixmap>
#include <QDebug>

#include "globals.h"

class DisplayLabel : public QLabel
{
    Q_OBJECT
public:
    explicit DisplayLabel(QWidget *parent = 0);
    void setImage(const QPixmap *image);

    void resizeEvent(QResizeEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void openCurrentDirectory(void);
public slots:

private:
    QPixmap _image;
};

#endif // DISPLAYLABEL_H
