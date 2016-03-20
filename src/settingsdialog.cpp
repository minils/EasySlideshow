#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    _settingsmanager = new SettingsManager();
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
    this->setVisible(false);
    emit settingsClosed();
}

void SettingsDialog::showEvent(QShowEvent*)
{
    ui->imagePathEdit->setText(_settingsmanager->readSetting(SETTING_PATH, QVariant("")).toString());
    ui->durationSpinBox->setValue(_settingsmanager->readSetting(SETTING_SPEED, QVariant(1)).toInt());
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

void SettingsDialog::on_imagePathButton_clicked()
{
//    QFileDialog *dialog = new QFileDialog();
//    dialog->setFileMode(QFileDialog::Directory);
//    dialog->setOption(QFileDialog::ShowDirsOnly);
//    dialog->show();
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
