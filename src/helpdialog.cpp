#include "helpdialog.h"
#include "ui_helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

    ui->infoLabel->setText(ui->infoLabel->text().arg(VERSION));
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::showEvent(QShowEvent*)
{
    // do sth
}
