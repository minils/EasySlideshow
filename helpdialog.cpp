#include "helpdialog.h"
#include "ui_helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
    QString info = ui->infoLabel->text();
    info = info.replace(QString("%version"), QString(VERSION));
    ui->infoLabel->setText(info);
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::showEvent(QShowEvent * event)
{
    // do sth
}
