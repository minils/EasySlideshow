#ifndef DETAILSDIALOG_H
#define DETAILSDIALOG_H

#include <QDialog>
#include <QFileInfo>
#include <QDir>
#include <QImageReader>
#include <QDebug>

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

private:
    Ui::DetailsDialog *ui;
};

#endif // DETAILSDIALOG_H
