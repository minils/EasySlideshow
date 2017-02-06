#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QDebug>
#include <QFontDatabase>
#include <QSettings>
#include <QVariant>
#include <QThread>

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
    bool _settingsShown;

    QDialog *_helpDialog;
    bool _helpShown;

    SlideShow *_slideshow;
    SettingsManager *_settingsManager;
    QTranslator *_currentTranslator;
    QString _currentLanguage;
    QPixmap *current_image;
    QPixmap *current_image_scaled;

    void loadSettings(void);
    void saveSettings(void);
    void controls(bool enable);

public slots:
    void folderStatusChanged(bool exists, bool readable);
    void loadImage(const QPixmap *image);
    void displayPath(QString path);
    void on_pauseButton_clicked(void);
    void updateImageCursor(void);

    void settingsClosed(bool accepted);
    void helpClosed(void);
    void changeLanguage(QString lang);

    void startedSlideshowInit(void);
    void stoppedSlideshowInit(void);

private slots:
    void on_nextButton_clicked(void);
    void on_previousButton_clicked(void);
    void on_settingsButton_clicked();
    void on_helpButton_clicked();

signals:
    void pausePressed(void);
    void nextImageClicked(void);
    void previousImageClicked(void);

protected:
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // MAINWINDOW_H
