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
    void closeEvent(QCloseEvent *event);

private slots:
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
    void on_SettingsDialog_accepted();
    void on_SettingsDialog_rejected();
    void onBrowseButtonClicked(QLineEdit *lineEdit);
    void removeLine(QHBoxLayout *layout);

    void on_addButton_clicked();

private:
    void showError(QString msg);
    void hideError(void);
    void createLanguageMenu(void);
    QPushButton* addPathEdit(QString);

    Ui::SettingsDialog *ui;
    SettingsManager *_settingsmanager;
    QList<QLineEdit> *pathEdits;
    unsigned int amountPaths;

signals:
    void settingsClosed(bool accepted);
    void languageChanged(QString lang);
};

#endif // SETTINGSDIALOG_H
