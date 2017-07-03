#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QDebug>
#include <QFontDatabase>
#include <QSettings>
#include <QVariant>
#include <QThread>
#include <QSizeGrip>
#include <QScreen>

#include "globals.h"
#include "settingsmanager.h"
#include "slideshow.h"
#include "settingsdialog.h"
#include "helpdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void resizeEvent(QResizeEvent *);

private:
    Ui::MainWindow *ui;

    QDialog *_settingsDialog;

    QDialog *_helpDialog;
    bool _helpShown;
    QString _path;

    SlideShow *_slideshow;
    QTranslator *_currentTranslator;
    QString _currentLanguage;
    QPixmap *current_image;
    QPixmap *current_image_scaled;

    int _mouseClickCoordinate[2];

    void loadSettings(void);
    void saveSettings(void);
    void controls(bool enable);

public slots:
    void displayError(QString msg);
    void displayPath(QString path);
    void updateImageCursor(void);

    void settingsClosed(bool accepted);
    void helpClosed(void);
    void changeLanguage(QString lang);

    void startedSlideshowInit(void);
    void stoppedSlideshowInit(void);
    void lockButtonClicked(void);
    void updatePauseButton(void);

    void processRightClick(bool status);

private slots:
    void on_nextButton_clicked(void);
    void on_previousButton_clicked(void);
    void on_settingsButton_clicked();
    void on_helpButton_clicked();
    void on_pauseButton_clicked(void);

signals:
    void pauseClicked(void);
    void nextImageClicked(void);
    void previousImageClicked(void);

protected:
    bool eventFilter(QObject *watched, QEvent *event);protected:
};

#endif // MAINWINDOW_H
