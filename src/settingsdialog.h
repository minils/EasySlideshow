#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>

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

    void showEvent(QShowEvent *event);
    void changeEvent(QEvent *event);

private slots:
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
    void on_imagePathButton_clicked();
    void on_SettingsDialog_accepted();
    void on_SettingsDialog_rejected();
    void on_plus_button_clicked();
    void on_browse_button_clicked();

private:
    void showError(QString msg);
    void hideError(void);
    void createLanguageMenu(void);
    void addPathEdit(QString);
    void removeOldPaths(void);

    Ui::SettingsDialog *ui;
    SettingsManager *_settingsmanager;
    QList<QLineEdit> *pathEdits;
    unsigned int amountPaths = 0;
    const unsigned int maxAmountPaths = 10;

signals:
    void settingsClosed(bool accepted);
    void languageChanged(QString lang);
};

#endif // SETTINGSDIALOG_H
