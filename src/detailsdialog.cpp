#include "detailsdialog.h"
#include "ui_detailsdialog.h"

DetailsDialog::DetailsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetailsDialog)
{
    ui->setupUi(this);
}

DetailsDialog::~DetailsDialog()
{
    delete ui;
}

void DetailsDialog::setImage(const SlideshowImage *image) {
    ui->filenameLineEdit->setText(image->filename());
    ui->pathLineEdit->setText(image->directory());

    // Convert filsize to human readable number
    double num = image->filesize();
    QStringList list;
    list << "KB" << "MB" << "GB" << "TB";
    QStringListIterator i(list);
    QString unit("B");

    while (num >= 1024.0 && i.hasNext()) {
        unit = i.next();
        num /= 1024.0;
    }
    ui->sizeLineEdit->setText(QString().setNum(num, 'f', 2)+" "+unit);

    ui->resolutionLineEdit->setText(QString::number(image->resolution().width()) + "x" + QString::number(image->resolution().height()));

    ui->creationLineEdit->setText(image->creationTime().toString( Qt::SystemLocaleLongDate));
}
