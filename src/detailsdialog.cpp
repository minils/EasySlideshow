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

void DetailsDialog::setFile(QString filename)
{
    ui->filenameLineEdit->setText(QFileInfo(filename).fileName());
    ui->pathLineEdit->setText(QDir::toNativeSeparators(QFileInfo(filename).absolutePath()));
    float num = QFileInfo(filename).size();
    QStringList list;
    list << "KB" << "MB" << "GB" << "TB";
    QStringListIterator i(list);
    QString unit("B");

    while (num >= 1024.0 && i.hasNext()) {
        unit = i.next();
        num /= 1024.0;
    }
    ui->sizeLineEdit->setText(QString().setNum(num, 'f', 2)+" "+unit);

    QImageReader reader(filename);
    ui->resolutionLineEdit->setText(QString::number(reader.size().width()) + "x" + QString::number(reader.size().height()));
}
