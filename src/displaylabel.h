#ifndef DISPLAYLABEL_H
#define DISPLAYLABEL_H

#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QMouseEvent>
#include <QMenu>

#include "globals.h"

class DisplayLabel : public QLabel
{
    Q_OBJECT
public:
    explicit DisplayLabel(QWidget *parent = 0);
    void setImage(const QPixmap *image);
    void clearImage(void);

    void resizeEvent(QResizeEvent *);
    void mouseReleaseEvent(QMouseEvent *event);
    void setBackgroundColor(QColor backgroundColor);
 protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

signals:
    void imageClicked(void);
public slots:

private:
    QPixmap _image;
    QColor _backgroundColor;
};

#endif // DISPLAYLABEL_H
