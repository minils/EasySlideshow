#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << "[MainWindow] Starting...";
    qApp->installEventFilter(this);

    _mouseClickCoordinate[0] = 0;
    _mouseClickCoordinate[1] = 0;

    dragStartPosition.setX(0);
    dragStartPosition.setY(0);

    QSize windowSize = SettingsManager::readSetting(SETTING_WINDOW_SIZE).toSize();
    resize(windowSize);

    QPoint windowPosition = SettingsManager::readSetting(SETTING_WINDOW_POSITION).toPoint();
    move(windowPosition);

    // init slideshowimage
    _slideshowimage = nullptr;

    // set language
    _currentTranslator = nullptr;
    changeLanguage(SettingsManager::readSetting(SETTING_LANGUAGE).toString());

    // set style
    ui->statusLabel->setMinimumWidth(40);
    ui->statusLabel->setAlignment(Qt::AlignCenter);

    ui->photoLabel->setMinimumHeight(36);
    ui->photoLabel->setBackgroundColor(QColor("#404244"));
    updateImageCursor();

    // is it working on screens with lower dpi?
    qreal dpi = QGuiApplication::primaryScreen()->physicalDotsPerInch();
    int buttonW, buttonH;
    if (dpi > 150) {
        buttonW = 32;
        buttonH = 32;
    } else {
        buttonW = 24;
        buttonH = 24;
    }

    ui->rotateLeftButton->setMaximumSize(buttonW, buttonH);
    ui->rotateLeftButton->setText("");
    ui->rotateLeftButton->setIcon(QIcon(":/btn/rotate_left.svg"));
    ui->rotateLeftButton->setIconSize(QSize(buttonW*0.75, buttonH*0.75));

    ui->rotateRightButton->setMaximumSize(buttonW, buttonH);
    ui->rotateRightButton->setText("");
    ui->rotateRightButton->setIcon(QIcon(":/btn/rotate_right.svg"));
    ui->rotateRightButton->setIconSize(QSize(buttonW*0.75, buttonH*0.75));

    ui->previousButton->setMaximumSize(buttonW, buttonH);
    ui->previousButton->setText("");
    ui->previousButton->setIcon(QIcon(":/btn/skip_previous.svg"));
    ui->previousButton->setIconSize(QSize(buttonW*0.75, buttonH*0.75));

    ui->pauseButton->setMaximumSize(buttonW, buttonH);
    ui->pauseButton->setText("");
    ui->pauseButton->setIcon(QIcon(":/btn/pause.svg"));
    ui->pauseButton->setIconSize(QSize(buttonW*0.7, buttonH*0.7));

    ui->nextButton->setMaximumSize(buttonW, buttonH);
    ui->nextButton->setText("");
    ui->nextButton->setIcon(QIcon(":/btn/skip_next.svg"));
    ui->nextButton->setIconSize(QSize(buttonW*0.75, buttonH*0.75));

    ui->helpButton->setMaximumSize(buttonW, buttonH);
    ui->helpButton->setText("");
    ui->helpButton->setIcon(QIcon(":/btn/info_outline.svg"));
    ui->helpButton->setIconSize(QSize(buttonW*0.625, buttonH*0.625));

    ui->settingsButton->setMaximumSize(buttonW, buttonH);
    ui->settingsButton->setText("");
    ui->settingsButton->setIcon(QIcon(":/btn/settings.svg"));
    ui->settingsButton->setIconSize(QSize(buttonW*0.625, buttonH*0.625));

    ui->lockButton->setMaximumSize(buttonW, buttonH);
    ui->lockButton->setText("");
    ui->lockButton->setIcon(QIcon(":/btn/lock_open.svg"));
    ui->lockButton->setIconSize(QSize(buttonW*0.625, buttonH*0.625));

    ui->closeButton->setMaximumSize(buttonW, buttonH);
    ui->closeButton->setText("");
    ui->closeButton->setIcon(QIcon(":/btn/close.svg"));
    ui->closeButton->setIconSize(QSize(buttonW*0.625, buttonH*0.625));

    _details_enabled = false;
    displayDetails(_details_enabled);

    // setup slideshow
    qDebug() << "[MainWindow] Setting up slideshow";
    QStringList dir_list = SettingsManager::readSetting(SETTING_PATHS).toStringList();
    QList<QDir> *dirs = new QList<QDir>();
    foreach (QString dir, dir_list) {
        dirs->append(QDir(dir));
    }

    _slideshow = new SlideShow(ui->photoLabel, dirs, SettingsManager::readSetting(SETTING_SPEED).toInt(), this);

    /* Slideshow */
    connect(_slideshow, SIGNAL(communicatePauseStatus()), this, SLOT(updatePauseButton()));
    connect(_slideshow, SIGNAL(showImage(const SlideshowImage*)), this, SLOT(updateImage(const SlideshowImage*)));
    connect(_slideshow, SIGNAL(initStart()), this, SLOT(startedSlideshowInit()));
    connect(_slideshow, SIGNAL(initStop()), this, SLOT(stoppedSlideshowInit()));
    connect(_slideshow, SIGNAL(displayError(QString)), this, SLOT(displayError(QString)));

    connect(this, SIGNAL(nextImageClicked()), _slideshow, SLOT(nextImageClicked()));
    connect(this, SIGNAL(previousImageClicked()), _slideshow, SLOT(previousImageClicked()));
    connect(this, SIGNAL(pauseClicked()), _slideshow, SLOT(pauseClicked()));

    /* DisplayLabel */
    connect(ui->photoLabel, SIGNAL(rightMouseSucces(bool)), this, SLOT(processRightClick(bool)));
    connect(ui->photoLabel, SIGNAL(doubleClicked()), this, SLOT(imageDoubleClicked()));
    connect(ui->photoLabel, SIGNAL(openDetails(const SlideshowImage*)), this, SLOT(toggleDetails(const SlideshowImage*)));

    /* Widgets */
    connect(ui->rotateLeftButton, SIGNAL(clicked()), _slideshow, SLOT(rotateCurrentImageLeft()));
    connect(ui->rotateRightButton, SIGNAL(clicked()), _slideshow, SLOT(rotateCurrentImageRight()));
    connect(ui->lockButton, SIGNAL(clicked()), this, SLOT(lockButtonClicked()));

    // generate help dialog
    _helpDialog = new HelpDialog(this);
    _helpShown = false;
    connect(_helpDialog, SIGNAL(rejected()), this, SLOT(helpClosed()));

    _settingsShown = false;

    QTimer::singleShot(100, _slideshow, SLOT(init()));
}

MainWindow::~MainWindow()
{
  delete _helpDialog;
  delete _slideshow;
  delete ui;
}

void MainWindow::updateImage(const SlideshowImage* image)
{
  _slideshowimage = image;
  if (image == nullptr)
      return;
  ui->statusLabel->setText(image->path());
  QFontMetrics metrics(ui->statusLabel->font());
  QString elidedText = metrics.elidedText(image->path(), Qt::ElideLeft, ui->statusLabel->width());
  ui->statusLabel->setText(elidedText);
  if (elidedText != image->path()) {
    ui->statusLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  } else {
    ui->statusLabel->setAlignment(Qt::AlignCenter);
  }
  // TODO: Add this as an option
  //setWindowTitle("EasySlideshow: " + path);
  if (_details_enabled) {
      updateDetails(image);
  }
}

void MainWindow::updatePauseButton()
{
  if (_slideshow->paused())
    ui->pauseButton->setIcon(QIcon(":/btn/play.svg"));
  else
    ui->pauseButton->setIcon(QIcon(":/btn/pause.svg"));
}

void MainWindow::processRightClick(bool status)
{
  if (!_slideshow->paused() && status) {
      emit pauseClicked();
    }
  if (_slideshow->paused() && !status) {
      emit pauseClicked();
    }
}

void MainWindow::on_pauseButton_clicked()
{
    emit pauseClicked();
}

void MainWindow::resizeEvent(QResizeEvent*)
{
    if (!_slideshow->scanningIsActive()) {
        updateImage(_slideshowimage);
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

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
            dragStartPosition = event->pos();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if ((event->pos() - dragStartPosition).manhattanLength()
            < QApplication::startDragDistance()) {
            return;
    }
    if (ui->photoLabel->geometry().contains(dragStartPosition)) {
        // Save pause status for later
        bool pauseStatus = _slideshow->paused();
        _slideshow->pauseSlideshow(true);

        // Handle the drag action
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        QList<QUrl> list;
        list.append(QUrl::fromLocalFile(_slideshowimage->path()));
        mimeData->setUrls(list);
        drag->setMimeData(mimeData);

        QPixmap pixmap = ui->photoLabel->pixmap()->scaled(QSize(200, 100), Qt::KeepAspectRatio);
        drag->setPixmap(pixmap);

        QPoint hs = drag->hotSpot();
        // +10 is a good guess for the width of the mouse pointer
        drag->setHotSpot(hs + QPoint(drag->pixmap().width()/2+10, drag->pixmap().height()/2+10));

        Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction | Qt::LinkAction, Qt::CopyAction);

        if (dropAction == Qt::IgnoreAction) {
            _slideshow->pauseSlideshow(pauseStatus);
        }
    }
}

void MainWindow::on_settingsButton_clicked()
{
    if (_settingsShown) {
        _settingsDialog->activateWindow();
        _settingsDialog->raise();
    } else {
        //ui->settingsButton->setEnabled(false);
        _settingsDialog = new SettingsDialog(this);
        connect(_settingsDialog, SIGNAL(settingsClosed(bool)), this, SLOT(settingsClosed(bool)));
        connect(_settingsDialog, SIGNAL(languageChanged(QString)), this, SLOT(changeLanguage(QString)));
        _settingsDialog->show();
        _settingsDialog->activateWindow();
        _settingsDialog->raise();
        _settingsShown = true;
    }
}

void MainWindow::settingsClosed(bool accepted)
{
    //ui->settingsButton->setEnabled(true);
    _settingsShown = false;
    if (!accepted)
        return;

    QStringList dir_list = SettingsManager::readSetting(SETTING_PATHS).toStringList();
    QList<QDir> *dirs = new QList<QDir>();
    foreach (QString dir, dir_list) {
        dirs->append(QDir(dir));
    }

    int duration = SettingsManager::readSetting(SETTING_SPEED).toInt();
    _slideshow->setSpeed(duration);
    _slideshow->setDirs(dirs);  // calls init if necessary
    updateImageCursor();
    delete _settingsDialog;
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
    if (SettingsManager::readSetting(SETTING_ON_CLICK_ACTION).toString()
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
    ui->statusLabel->setAlignment(Qt::AlignCenter);
    ui->statusLabel->setText(tr("Scanning folders..."));
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
    } else if (event->type() == QEvent::Close) {
      SettingsManager::writeSetting(SETTING_WINDOW_SIZE, QVariant(geometry().size()));
      SettingsManager::writeSetting(SETTING_WINDOW_POSITION, QVariant(frameGeometry().topLeft()));
      event->accept();
    } else if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = (QMouseEvent*) event;
        if (mouseEvent->button() == Qt::LeftButton && obj == ui->photoLabel) {
            if (SettingsManager::readSetting(SETTING_ON_CLICK_ACTION).toString()
                    != SETTING_ON_CLICK_ACTION_NOTHING)
                on_pauseButton_clicked();
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
    flags &= ~Qt::X11BypassWindowManagerHint;
    ui->lockButton->setIcon(QIcon(":/btn/lock_open"));
  } else {
    flags |= Qt::WindowStaysOnTopHint;
    flags |= Qt::X11BypassWindowManagerHint;
    ui->lockButton->setIcon(QIcon(":/btn/lock"));
  }
  this->setWindowFlags(flags);
  restoreState(backup);
  show();
}

void MainWindow::displayError(QString msg)
{
    ui->photoLabel->clearImage();
    ui->statusLabel->setText(msg);
    controls(false);
    // TODO: controls should be disabled
}

void MainWindow::imageDoubleClicked(void) {
    if (isFullScreen()) {
        showNormal();
    } else {
        showFullScreen();
    }
}

void MainWindow::displayDetails(bool enable) {
    ui->closeButton->setVisible(enable);
    ui->DetailsLabel->setVisible(enable);
    ui->filenameGroup->setVisible(enable);
    ui->filenameLineEdit->setVisible(enable);
    ui->pathGroup->setVisible(enable);
    ui->pathLineEdit->setVisible(enable);
    ui->sizeGroup->setVisible(enable);
    ui->sizeLineEdit->setVisible(enable);
    ui->resolutionGroup->setVisible(enable);
    ui->resolutionLineEdit->setVisible(enable);
    ui->creationDateGroup->setVisible(enable);
    ui->creationDateLineEdit->setVisible(enable);
    ui->descriptionGroup->setVisible(enable);
    ui->descriptionTextBrowser->setVisible(enable);
    _details_enabled = enable;
}

void MainWindow::toggleDetails(const SlideshowImage* image) {
    displayDetails(!_details_enabled);
    updateDetails(_slideshowimage);
}

void MainWindow::updateDetails(const SlideshowImage* image) const {
    ui->filenameLineEdit->setText(image->filename());
    ui->pathLineEdit->setText(image->directory());
    double num = image->filesize();
    QStringList list;
    list << "KB" << "MB" << "GB" << "TB";
    QStringListIterator i(list);
    QString unit("B");

    while (num >= 1024.0 && i.hasNext()) {
        unit = i.next();
        num /= 1024.0;
    }
    ui->sizeLineEdit->setText(QString().setNum(num, 'f', 2)+" "+unit);

    ui->resolutionLineEdit->setText(QString::number(image->resolution().width()) + "x" + QString::number(image->resolution().height()));

    ui->creationDateLineEdit->setText(image->creationTime().toString( Qt::SystemLocaleLongDate));
}

void MainWindow::on_closeButton_clicked()
{
    displayDetails(false);
}
