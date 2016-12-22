#include "helpdialog.h"
#include "ui_helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

#ifdef GIT_COMMIT
    ui->infoLabel->setText(ui->infoLabel->text().arg(VERSION, GIT_COMMIT));
#else
    ui->infoLabel->setText(ui->infoLabel->text().arg(VERSION, "0"));
#endif
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::showEvent(QShowEvent*)
{
    // do sth
}
