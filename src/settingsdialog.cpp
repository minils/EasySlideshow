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


    connect(ui->plusDuration, &QPushButton::clicked,
            this, [this]() {this->ui->durationSpinBox->setValue(this->ui->durationSpinBox->value()+1);});
    connect(ui->minusDuration, &QPushButton::clicked,
            this, [this]() {this->ui->durationSpinBox->setValue(this->ui->durationSpinBox->value()-1);});


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
    QLayoutItem *child;
    while((child = ui->pathHolderLayout->takeAt(0)) != 0) {
        QLayoutItem *grandchild;
        grandchild = ((QLayout* )child)->takeAt(0);
        if (grandchild != 0) {
            QWidget *path = grandchild->widget();
            QString p = ((QLineEdit *) path)->text();
            if (!p.isEmpty())
                dirs.append(p);
        }
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
    delete this;
    emit settingsClosed(true);
}

void SettingsDialog::showEvent(QShowEvent *event)
{
    QStringList dirs = _settingsmanager->readSetting(SETTING_PATHS).toStringList();

    // create lineEdits for every path
    amountPaths = 0;
    for (int i = 0; i < dirs.size(); i++) {
        addPathEdit(dirs.at(i));
    }

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

void SettingsDialog::on_SettingsDialog_accepted()
{
    qDebug() << "accepted";
}

void SettingsDialog::on_SettingsDialog_rejected()
{
    on_buttonBox_rejected();
}

QPushButton* SettingsDialog::addPathEdit(QString dir)
{
    if (amountPaths > maxAmountPaths) {
        showError(tr("Cannot add more than %1 paths").arg(maxAmountPaths));
        return NULL;
    }
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setObjectName("pathLine");
    hLayout->setSizeConstraint(QHBoxLayout::SetMinimumSize);

    QLineEdit *lineEdit = new QLineEdit(dir);
    lineEdit->setMinimumHeight(25);
    lineEdit->setObjectName("path");
    if (!dir.isEmpty())
        lineEdit->setToolTip(dir);
    hLayout->addWidget(lineEdit);

    QPushButton *browseButton = new QPushButton(tr("Browse..."));
    browseButton->setMinimumHeight(28);
    hLayout->addWidget(browseButton);
    connect(browseButton, &QPushButton::clicked,
            lineEdit, [this, lineEdit]() {this->onBrowseButtonClicked(lineEdit);});

    QPushButton *minusButton = new QPushButton("-");
    minusButton->setMinimumHeight(28);
    minusButton->setMaximumWidth(28);
    minusButton->setObjectName("minusButton");
    hLayout->addWidget(minusButton);
    connect(minusButton, &QPushButton::clicked,
            hLayout, [this, hLayout]() {this->removeLine(hLayout);});

    ui->pathHolderLayout->addLayout(hLayout);
    amountPaths++;
    return browseButton;
}

void SettingsDialog::onBrowseButtonClicked(QLineEdit *lineEdit)
{
    QString previous_dir = lineEdit->text();
    if (!QDir(previous_dir).exists()) {
        previous_dir = QDir::homePath();
    }
    QString dir = QFileDialog::getExistingDirectory(this, tr("Pick a folder"), previous_dir);
    if (!dir.isEmpty()) {
        lineEdit->setText(dir);
        lineEdit->setToolTip(dir);
        hideError();
    }
}

void SettingsDialog::removeLine(QHBoxLayout *layout)
{
    if (amountPaths == 1) {
        showError(tr("At least one path is needed"));
        return;
    }
    QLayoutItem *child;
    while((child = layout->takeAt(0)) != 0) {
        if (child->widget()) {
            child->widget()->setParent(NULL);
            delete child;
        }
    }
    amountPaths--;
}

void SettingsDialog::on_addButton_clicked()
{
    addPathEdit("");
}
