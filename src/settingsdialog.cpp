#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    _settingsmanager = new SettingsManager();
    createLanguageMenu();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_rejected()
{
    this->hide();
    emit settingsClosed();
}

void SettingsDialog::on_buttonBox_accepted()
{
    QDir dir = QDir(ui->imagePathEdit->text());
    int duration = ui->durationSpinBox->value();

    if (duration <= 0) {
        showError("Duration must be above 0");
        return;
    }

    if (!dir.exists()) {
        showError("Directory does not exist.");
        return;
    }

    _settingsmanager->writeSetting(SETTING_PATH, QVariant(dir.absolutePath()));
    _settingsmanager->writeSetting(SETTING_SPEED, QVariant(duration));

    QVariant settingClick;
    if (ui->radioNothing->isChecked()) {
        settingClick = QVariant(SETTING_ON_CLICK_ACTION_NOTHING);
    } else if (ui->radioOpenFolder->isChecked()) {
        settingClick = QVariant(SETTING_ON_CLICK_ACTION_OPEN_FOLDER);
    } else if (ui->radioPause->isChecked()) {
        settingClick = QVariant(SETTING_ON_CLICK_ACTION_PAUSE);
    }
    _settingsmanager->writeSetting(SETTING_ON_CLICK_ACTION, settingClick);

    this->setVisible(false);
    emit settingsClosed();
}

void SettingsDialog::showEvent(QShowEvent*)
{
    ui->imagePathEdit->setText(_settingsmanager->readSetting(SETTING_PATH).toString());
    ui->durationSpinBox->setValue(_settingsmanager->readSetting(SETTING_SPEED).toInt());
    QString settingClick = _settingsmanager->readSetting(SETTING_ON_CLICK_ACTION).toString();
        ui->radioNothing->setChecked(settingClick == SETTING_ON_CLICK_ACTION_NOTHING);
        ui->radioOpenFolder->setChecked(settingClick == SETTING_ON_CLICK_ACTION_OPEN_FOLDER);
        ui->radioPause->setChecked(settingClick == SETTING_ON_CLICK_ACTION_PAUSE);
    hideError();
    ui->buttonBox->setFocus();
}

void SettingsDialog::showError(QString msg)
{
    ui->errorLabel->setVisible(true);
    ui->errorLabel->setStyleSheet("QLabel { color: #F00; }");
    ui->errorLabel->setText(msg);
}

void SettingsDialog::hideError(void)
{
    ui->errorLabel->setVisible(false);
}

void SettingsDialog::createLanguageMenu()
{
    qDebug() << "createlanguageMenu()";
    QString defaultLocale = QLocale::system().name();       // e.g. de_DE
    defaultLocale.truncate(defaultLocale.lastIndexOf("_")); // e.g. de

    QString langPath = QApplication::applicationDirPath().append("/l10n");
    QDir dir(langPath);
    QStringList fileNames = dir.entryList(QStringList("EasySlideshow_*.qm"));
    for (int i = 0; i < fileNames.size(); ++i) {
        qDebug() << fileNames[i];
        QString locale;
        locale = fileNames[i];
        locale.truncate(locale.lastIndexOf("."));       // e.g. EasySlideshow_de
        locale.remove(0, locale.lastIndexOf("_") + 1);  // e.g. de
        QString lang = QLocale::languageToString(QLocale(locale).language());
        ui->languageSelector->addItem(lang, QVariant(locale));
        if (defaultLocale == locale) {
            ui->languageSelector->setCurrentIndex(i);
        }
    }

}

void SettingsDialog::on_imagePathButton_clicked()
{
    QString previous_dir = ui->imagePathEdit->text();
    if (!QDir(previous_dir).exists()) {
        previous_dir = QDir::homePath();
    }
    QString dir = QFileDialog::getExistingDirectory(this, "Pick folder", previous_dir);
    if (!dir.isEmpty()) {
        ui->imagePathEdit->setText(dir);
        hideError();
    }
}

void SettingsDialog::on_SettingsDialog_accepted()
{
    qDebug() << "accepted";
}

void SettingsDialog::on_SettingsDialog_rejected()
{
    on_buttonBox_rejected();
}
