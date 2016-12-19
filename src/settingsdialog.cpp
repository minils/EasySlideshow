#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include<limits>

const unsigned int maxAmountPaths = 10;

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    _settingsmanager = new SettingsManager();

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("OK"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
    ui->imagePathButton->setText(tr("Browse..."));

    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(on_plus_button_clicked()));

    createLanguageMenu();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_rejected()
{
    this->hide();
    emit settingsClosed(false);
}

void SettingsDialog::on_buttonBox_accepted()
{
    int duration = ui->durationSpinBox->value();

    if (duration <= 0) {
        showError("Duration must be above 0");
        return;
    } else if ((unsigned int)duration >= std::numeric_limits<unsigned int>::max()) {
        showError(QString("Duration must be below %1").arg(std::numeric_limits<unsigned int>::max()));
    }

    _settingsmanager->writeSetting(SETTING_SPEED, QVariant(duration));

    QStringList dirs;
    dirs.append(QDir(ui->imagePathEdit->text()).absolutePath());
    QList<QLineEdit *> children = ui->centralWidget->findChildren<QLineEdit *>("path");
    foreach (QLineEdit* child, children) {
        QString p = child->text().trimmed();
        if (!p.isEmpty())
            dirs.append(p);
    }
    dirs.removeDuplicates();
    foreach (QString dir, dirs) {
        if (!QDir(dir).exists()) {
            showError("Directory does not exist.");
            return;
        }
    }

    _settingsmanager->writeSetting(SETTING_PATHS, QVariant(dirs));

    QVariant settingClick;
    if (ui->radioNothing->isChecked()) {
        settingClick = QVariant(SETTING_ON_CLICK_ACTION_NOTHING);
    } else if (ui->radioOpenFolder->isChecked()) {
        settingClick = QVariant(SETTING_ON_CLICK_ACTION_OPEN_FOLDER);
    } else if (ui->radioPause->isChecked()) {
        settingClick = QVariant(SETTING_ON_CLICK_ACTION_PAUSE);
    }
    _settingsmanager->writeSetting(SETTING_ON_CLICK_ACTION, settingClick);

    QVariant lang = ui->languageSelector->itemData(ui->languageSelector->currentIndex());
    _settingsmanager->writeSetting(SETTING_LANGUAGE, lang);
    emit languageChanged(lang.toString());

    this->setVisible(false);
    emit settingsClosed(true);
}

void SettingsDialog::showEvent(QShowEvent *event)
{
    QStringList dirs = _settingsmanager->readSetting(SETTING_PATHS).toStringList();

    // create lineEdits for every path
    removeOldPaths();
    amountPaths = 0;
    if (dirs.size() > 0) {
        ui->imagePathEdit->setText(dirs.at(0));
    }
    for (int i = 1; i < dirs.size(); i++) {
        addPathEdit(dirs.at(i));
    }
    //this->adjustSize();

    ui->imagePathEdit->setText(dirs[0]);

    ui->durationSpinBox->setValue(_settingsmanager->readSetting(SETTING_SPEED).toInt());
    QString settingClick = _settingsmanager->readSetting(SETTING_ON_CLICK_ACTION).toString();
        ui->radioNothing->setChecked(settingClick == SETTING_ON_CLICK_ACTION_NOTHING);
        ui->radioOpenFolder->setChecked(settingClick == SETTING_ON_CLICK_ACTION_OPEN_FOLDER);
        ui->radioPause->setChecked(settingClick == SETTING_ON_CLICK_ACTION_PAUSE);
    createLanguageMenu();

    hideError();
    ui->buttonBox->setFocus();

    QDialog::showEvent(event);
}

void SettingsDialog::changeEvent(QEvent *event)
{
    if (event != 0) {
        if (event->type() == QEvent::LanguageChange) {
            ui->retranslateUi(this);
        }
    }
    QDialog::changeEvent(event);
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
    ui->languageSelector->clear();

    QString currentLocale = _settingsmanager->readSetting(SETTING_LANGUAGE).toString();

    QDir dir(":/l10n");
    QStringList fileNames = dir.entryList(QStringList("*.qm"));
    for (int i = 0; i < fileNames.size(); ++i) {
        QString locale;
        locale = fileNames[i];
        locale.truncate(locale.lastIndexOf("."));       // e.g. EasySlideshow_de
        locale.remove(0, locale.lastIndexOf("_") + 1);  // e.g. de
        QString lang = QLocale::languageToString(QLocale(locale).language());
        // TODO: Display language name in own language
        ui->languageSelector->addItem(lang, QVariant(locale));

        QString flag = QLocale(locale).name();
        flag.remove(0, flag.lastIndexOf("_") + 1);
        flag = ":/icon/flags/" + flag.toLower() + ".png";
        ui->languageSelector->setItemIcon(i, QIcon(flag));


        if (currentLocale == locale) {
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

void SettingsDialog::addPathEdit(QString dir)
{
    if (amountPaths > maxAmountPaths) {
        showError(tr("Cannot add more than %1 paths").arg(maxAmountPaths));
        return;
    }
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setObjectName("pathLine");
    hLayout->setSizeConstraint(QHBoxLayout::SetMinimumSize);

    QLineEdit *lineEdit = new QLineEdit(dir);
    lineEdit->setMinimumHeight(25);
    lineEdit->setObjectName("path");
    hLayout->addWidget(lineEdit);

    QPushButton *browseButton = new QPushButton(tr("Browse..."));
    browseButton->setMinimumHeight(28);
    browseButton->setObjectName("browseButton");
    hLayout->addWidget(browseButton);
    connect(browseButton, SIGNAL(clicked()), this, SLOT(on_browse_button_clicked()));

    ui->pathHolderLayout->addLayout(hLayout);
    amountPaths++;
}

void SettingsDialog::on_plus_button_clicked()
{
    addPathEdit("");
}

void SettingsDialog::on_browse_button_clicked()
{
    // find corresponding lineEdit:
    QLineEdit *pathEdit;
    QPushButton *b = qobject_cast<QPushButton *>(sender());
    QList<QPushButton *> children = ui->centralWidget->findChildren<QPushButton *>("browseButton");
    QList<QLineEdit *> lineEdits = ui->centralWidget->findChildren<QLineEdit *>("path");
    Q_ASSERT(lineEdits.length() == children.length());
    for (int i = 0; i < children.length(); i++) {
        if (children[i] == b) {
            pathEdit = lineEdits[i];
            break;
        }
    }
    QString previous_dir = pathEdit->text();
    if (!QDir(previous_dir).exists()) {
        previous_dir = QDir::homePath();
    }
    QString dir = QFileDialog::getExistingDirectory(this, "Pick folder", previous_dir);
    if (!dir.isEmpty()) {
        pathEdit->setText(dir);
        hideError();
    }
}

void SettingsDialog::removeOldPaths()
{
    QStringList list;
    list.append("path"); list.append("browseButton");
    foreach (QString s, list) {
        QList<QObject *> children = ui->centralWidget->findChildren<QObject *>(s);
        foreach (QObject* child, children) {
            delete child;
        }
    }
}
