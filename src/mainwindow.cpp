#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);
    qDebug() << "[MainWindow] Starting... " << QThread::currentThreadId();
    qApp->installEventFilter(this);

    _mouseClickCoordinate[0] = 0;
    _mouseClickCoordinate[1] = 0;

    int id = QFontDatabase::addApplicationFont(":/font/roboto");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont roboto12(family, 12);
    ui->statusLabel->setFont(roboto12);

    _settingsManager = new SettingsManager();

    // set language
    _currentTranslator = NULL;
    changeLanguage(_settingsManager->readSetting(SETTING_LANGUAGE).toString());

    // set style
    ui->statusLabel->setMinimumWidth(40);
    //ui->statusLabel->setAlignment(Qt::AlignRight);

    ui->photoLabel->setMinimumHeight(36);
    updateImageCursor();

    ui->rotateLeftButton->setText("");
    ui->rotateLeftButton->setIcon(QIcon(":/btn/rotate_left"));
    ui->rotateLeftButton->setIconSize(QSize(20, 20));

    ui->rotateRightButton->setText("");
    ui->rotateRightButton->setIcon(QIcon(":/btn/rotate_right"));
    ui->rotateRightButton->setIconSize(QSize(20, 20));

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

    ui->lockButton->setText("");
    ui->lockButton->setIcon(QIcon(":/btn/lock_open"));
    ui->lockButton->setIconSize(QSize(18, 18));

    // setup slideshow
    qDebug() << "[MainWindow] Setting up slideshow";
    QStringList dir_list = _settingsManager->readSetting(SETTING_PATHS).toStringList();
    QList<QDir> *dirs = new QList<QDir>();
    foreach (QString dir, dir_list) {
        dirs->append(QDir(dir));
    }

    _slideshow = new SlideShow(dirs, _settingsManager->readSetting(SETTING_SPEED).toInt(), this);

    /* Slideshow */
    connect(_slideshow, SIGNAL(dirChecked(bool, bool)), this, SLOT(folderStatusChanged(bool, bool)));
    connect(_slideshow, SIGNAL(showImage(const QPixmap*)), this, SLOT(loadImage(const QPixmap*)));
    connect(_slideshow, SIGNAL(communicatePause()), this, SLOT(on_pauseButton_clicked()));
    connect(_slideshow, SIGNAL(showPath(QString)), this, SLOT(displayPath(QString)));
    connect(_slideshow, SIGNAL(initStart()), this, SLOT(startedSlideshowInit()));
    connect(_slideshow, SIGNAL(initStop()), this, SLOT(stoppedSlideshowInit()));

    connect(this, SIGNAL(nextImageClicked()), _slideshow, SLOT(nextImageClicked()));
    connect(this, SIGNAL(previousImageClicked()), _slideshow, SLOT(previousImageClicked()));
    connect(this, SIGNAL(pausePressed()), _slideshow, SLOT(pause()));

    /* Widgets */
    connect(ui->photoLabel, SIGNAL(imageClicked()), _slideshow, SLOT(imageClicked()));
    connect(ui->rotateLeftButton, SIGNAL(clicked()), _slideshow, SLOT(rotateCurrentImageLeft()));
    connect(ui->rotateRightButton, SIGNAL(clicked()), _slideshow, SLOT(rotateCurrentImageRight()));
    connect(ui->lockButton, SIGNAL(clicked()), this, SLOT(lockButtonClicked()));

    // generate settings dialog
    _settingsDialog = new SettingsDialog(this);
    _settingsShown = false;
    connect(_settingsDialog, SIGNAL(settingsClosed(bool)), this, SLOT(settingsClosed(bool)));
    connect(_settingsDialog, SIGNAL(languageChanged(QString)), this, SLOT(changeLanguage(QString)));

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
  qDebug() << "Display path: " << path << ", width=" << ui->statusLabel->width();
  _path = path;
  ui->statusLabel->setText(path);
  QFontMetrics metrics(ui->statusLabel->font());
  QString elidedText = metrics.elidedText(path, Qt::ElideLeft, ui->statusLabel->width());
  ui->statusLabel->setText(elidedText);
  // TODO: Add this as an option
  //setWindowTitle("EasySlideshow: " + path);
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
    if (!_slideshow->scanningIsActive()) {
	displayPath(_path);
    }
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

void MainWindow::settingsClosed(bool accepted)
{
    _settingsShown = false;

    if (!accepted)
        return;

    QStringList dir_list = _settingsManager->readSetting(SETTING_PATHS).toStringList();
    QList<QDir> *dirs = new QList<QDir>();
    foreach (QString dir, dir_list) {
        dirs->append(QDir(dir));
    }

    int duration = _settingsManager->readSetting(SETTING_SPEED).toInt();
    _slideshow->setSpeed(duration);
    _slideshow->setDirs(dirs);
    updateImageCursor();
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

void MainWindow::updateImageCursor(void)
{
    if (_settingsManager->readSetting(SETTING_ON_CLICK_ACTION).toString()
            == SETTING_ON_CLICK_ACTION_NOTHING) {
        ui->photoLabel->setCursor(Qt::ArrowCursor);
    } else {
         ui->photoLabel->setCursor(Qt::PointingHandCursor);
    }
}

void MainWindow::changeLanguage(QString lang)
{
    if (lang == _currentLanguage)
        return;
    _currentLanguage = lang;
    bool first = false;
    if (_currentTranslator != NULL) {
        qApp->removeTranslator(_currentTranslator);
    } else {
        _currentTranslator = new QTranslator();
        first = true;
    }
    if (_currentTranslator->load(QString(":/l10n/%1.qm").arg(lang))) {
        qApp->installTranslator(_currentTranslator);
        // update mainwindow
        ui->retranslateUi(this);
        QString language = QLocale::languageToString(QLocale(lang).language());
        if (!first) {
            ui->statusLabel->setText(tr("Changed language to <b>%1</b>").arg(language));
        }
        ui->previousButton->setText("");
        ui->pauseButton->setText("");
        ui->nextButton->setText("");
        ui->helpButton->setText("");
        ui->settingsButton->setText("");

    } else {
        qDebug() << "[Main] Error loading translation";
    }
}

void MainWindow::startedSlideshowInit()
{
    controls(false);
    ui->statusLabel->setText(tr("Scanning folders..."));
    ui->photoLabel->setText(tr("Loading..."));
}

void MainWindow::stoppedSlideshowInit()
{
    ui->pauseButton->setIcon(QIcon(":/btn/pause"));
    controls(true);
}

void MainWindow::controls(bool enable)
{
    ui->pauseButton->setEnabled(enable);
    ui->previousButton->setEnabled(enable);
    ui->nextButton->setEnabled(enable);
    ui->rotateLeftButton->setEnabled(enable);
    ui->rotateRightButton->setEnabled(enable);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        if (obj == this) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            switch(keyEvent->key()) {
            case Qt::Key_Right:
                on_nextButton_clicked();
                break;
            case Qt::Key_Left:
                on_previousButton_clicked();
                break;
            case Qt::Key_Space:
                on_pauseButton_clicked();
                break;
            case Qt::Key_L:
                ui->rotateLeftButton->click();
                break;
            case Qt::Key_R:
                ui->rotateRightButton->click();
                break;
            case Qt::Key_Q:
                this->close();
            default:
                break;
            }
        }
    }

    return QObject::eventFilter(obj, event);
}

void MainWindow::lockButtonClicked(void)
{
  QByteArray backup = saveState();
  Qt::WindowFlags flags = this->windowFlags();
  if (this->windowFlags() & Qt::WindowStaysOnTopHint) {
    flags &= ~Qt::WindowStaysOnTopHint;
    ui->lockButton->setIcon(QIcon(":/btn/lock_open"));
  } else {
    flags |= Qt::WindowStaysOnTopHint;
    ui->lockButton->setIcon(QIcon(":/btn/lock"));
  }
  qDebug() << this->windowFlags();
  this->setWindowFlags(flags);
  restoreState(backup);
  show();
}
