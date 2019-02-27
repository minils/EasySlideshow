#ifndef DETAILSDIALOG_H
#define DETAILSDIALOG_H

#include <QDialog>
#include <QFileInfo>
#include <QDir>
#include <QImageReader>
#include <QDebug>
#include <QDateTime>

#include "slideshowimage.h"

namespace Ui {
class DetailsDialog;
}

class DetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DetailsDialog(QWidget *parent = 0);
    ~DetailsDialog();

    void setFile(QString filename);
    void setImage(const SlideshowImage* image);

private:
    Ui::DetailsDialog *ui;

    void forEachExif(ExifContent* ec, void* user_data);
};

#endif // DETAILSDIALOG_H
