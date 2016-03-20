#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _settingsManager = new SettingsManager();

    // set style
    ui->statusLabel->setMinimumWidth(40);
    ui->statusLabel->setAlignment(Qt::AlignRight);
    ui->photoLabel->setMinimumHeight(36);

    ui->previousButton->setText("");
    ui->previousButton->setIcon(QIcon(":/btn/previous"));
    ui->previousButton->setIconSize(QSize(20, 20));

    ui->pauseButton->setText("");
    ui->pauseButton->setIcon(QIcon(":/btn/pause"));
    ui->pauseButton->setIconSize(QSize(20, 20));

    ui->nextButton->setText("");
    ui->nextButton->setIcon(QIcon(":/btn/next"));
    ui->nextButton->setIconSize(QSize(20, 20));

    ui->helpButton->setText("");
    ui->helpButton->setIcon(QIcon(":/btn/info"));
    ui->helpButton->setIconSize(QSize(18, 18));

    ui->settingsButton->setText("");
    ui->settingsButton->setIcon(QIcon(":/btn/settings"));
    ui->settingsButton->setIconSize(QSize(18, 18));



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

    // generate help dialog
    _helpDialog = new HelpDialog(this);
    _helpShown = false;
    connect(_helpDialog, SIGNAL(rejected()), this, SLOT(helpClosed()));


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
    if (_slideshow->paused()) {
        ui->pauseButton->setIcon(QIcon(":/btn/pause"));
    } else {
        ui->pauseButton->setIcon(QIcon(":/btn/play"));
    }
    emit pausePressed();
}


void MainWindow::resizeEvent(QResizeEvent*)
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

void MainWindow::on_helpButton_clicked()
{
    if (!_helpShown) {
        _helpDialog->show();
        _helpDialog->activateWindow();
        _helpDialog->raise();
        _helpShown = true;
    } else {
        _helpDialog->activateWindow();
        _helpDialog->raise();
    }
}

void MainWindow::helpClosed()
{
    _helpShown = false;
}
