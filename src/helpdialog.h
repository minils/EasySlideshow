#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QFile>

#include "globals.h"

namespace Ui {
class HelpDialog;
}

class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HelpDialog(QWidget *parent = 0);
    ~HelpDialog();

    void showEvent(QShowEvent *);

private:
    Ui::HelpDialog *ui;

signals:
    void helpClosed(void);
};

#endif // HELPDIALOG_H
