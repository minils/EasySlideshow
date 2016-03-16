#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QFileDialog>

#include "settingsmanager.h"
#include "globals.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

    void showEvent(QShowEvent * event);

private slots:

    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

    void on_imagePathButton_clicked();

    void on_SettingsDialog_accepted();

    void on_SettingsDialog_rejected();

private:
    void showError(QString msg);
    void hideError(void);

    Ui::SettingsDialog *ui;
    SettingsManager *_settingsmanager;

signals:
    void settingsClosed(void);
};

#endif // SETTINGSDIALOG_H