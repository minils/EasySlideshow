#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _play = "I>";
    _pause = "II";

    _settingsManager = new SettingsManager();

    // set style
    ui->statusLabel->setStyleSheet("QLabel { background-color: #" + QString(BACKGROUND_COLOR) + "; color: #" + QString(FONT_COLOR) + "; }");
    ui->statusLabel->setMinimumWidth(40);
    ui->statusLabel->setAlignment(Qt::AlignRight);
    ui->photoLabel->setMinimumHeight(36);
    ui->centralWidget->setStyleSheet("QWidget { background-color: #" + QString(BACKGROUND_COLOR) + "; }");
    ui->pauseButton->setStyleSheet("QPushButton { color: #" + QString(FONT_COLOR) + "; }");
    ui->helpButton->setStyleSheet("QPushButton { color: #" + QString(FONT_COLOR) + "; }");
    ui->previousButton->setStyleSheet("QPushButton { color: #" + QString(FONT_COLOR) + "; }");
    ui->nextButton->setStyleSheet("QPushButton { color: #" + QString(FONT_COLOR) + "; }");
    ui->settingsButton->setStyleSheet("QPushButton { color: #" + QString(FONT_COLOR) + "; }");

    // setup slideshow
    qDebug() << "[MainWindow] Setting up slideshow";
    QDir* dir = new QDir(_settingsManager->readSetting(SETTING_PATH, QVariant("")).toString());
    _slideshow = new SlideShow(dir, _settingsManager->readSetting(SETTING_SPEED, QVariant(6)).toInt(), this);

    connect(_slideshow, SIGNAL(dirChecked(bool, bool)), this, SLOT(folderStatusChanged(bool, bool)));
    connect(_slideshow, SIGNAL(showImage(const QPixmap*)), this, SLOT(loadImage(const QPixmap*)));
    connect(_slideshow, SIGNAL(communicatePause()), this, SLOT(on_pauseButton_clicked()));
    connect(_slideshow, SIGNAL(showPath(QString)), this, SLOT(displayPath(QString)));

    connect(this, SIGNAL(nextImageClicked()), _slideshow, SLOT(nextImageClicked()));
    connect(this, SIGNAL(previousImageClicked()), _slideshow, SLOT(previousImageClicked()));
    connect(this, SIGNAL(pausePressed()), _slideshow, SLOT(pause()));

    connect(ui->photoLabel, SIGNAL(openCurrentDirectory()), _slideshow, SLOT(openCurrentDirectory()));

    // generate settings dialog
    _settingsDialog = new SettingsDialog(this);
    _settingsShown = false;
    connect(_settingsDialog, SIGNAL(settingsClosed()), this, SLOT(settingsClosed()));

    QTimer::singleShot(100, _slideshow, SLOT(init()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// TODO: flag if no fotos were found
void MainWindow::folderStatusChanged(bool exists, bool readable)
{
    if (!exists) {
        ui->statusLabel->setText("Folder could not be found.");
    } else if (!readable) {
        ui->statusLabel->setText("Folder could not be read.");
    }
}

void MainWindow::loadImage(const QPixmap *image)
{
    ui->photoLabel->setImage(image);
}

void MainWindow::displayPath(QString path)
{
    ui->statusLabel->setText(path);
}

void MainWindow::on_pauseButton_clicked()
{
    if (ui->pauseButton->text() == _pause){
        ui->pauseButton->setText(_play);
    } else {
        ui->pauseButton->setText(_pause);
    }
    emit pausePressed();
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    int width = ui->centralWidget->width();
    ui->statusLabel->setMaximumWidth(width-80); // 80 is the size of the 4 buttons
}

void MainWindow::on_nextButton_clicked(void)
{
    emit nextImageClicked();
}

void MainWindow::on_previousButton_clicked(void)
{
    emit previousImageClicked();
}

void MainWindow::on_settingsButton_clicked()
{
    if (!_settingsShown) {
        _settingsDialog->show();
        _settingsDialog->activateWindow();
        _settingsDialog->raise();
        _settingsShown = true;
    } else {
        _settingsDialog->activateWindow();
        _settingsDialog->raise();
    }
}

void MainWindow::settingsClosed(void)
{
    _settingsShown = false;
    QDir* dir = new QDir(_settingsManager->readSetting(SETTING_PATH, QVariant("")).toString());
    int duration = _settingsManager->readSetting(SETTING_SPEED, QVariant(6)).toInt();
    _slideshow->setSpeed(duration);
    _slideshow->setDirectory(dir);
}
