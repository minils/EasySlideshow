#ifndef DISPLAYLABEL_H
#define DISPLAYLABEL_H

#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QMouseEvent>
#include <QMenu>
#include <QDesktopServices>

#include "globals.h"
#include "settingsmanager.h"

class DisplayLabel : public QLabel
{
    Q_OBJECT
public:
    explicit DisplayLabel(QWidget *parent = 0);
    void clearImage(void);
    void displayImage(QString path, int degree);

    void resizeEvent(QResizeEvent *);
    void mouseReleaseEvent(QMouseEvent *event);
    void setBackgroundColor(QColor backgroundColor);
protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
private:
    QPixmap _image;
    QColor _backgroundColor;
    QString _path;

    void openFolder(bool ignoreSettings);
    void openImage(void);
};

#endif // DISPLAYLABEL_H
