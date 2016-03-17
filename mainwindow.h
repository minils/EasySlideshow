#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QDebug>
#include <QFontDatabase>
#include <QSettings>
#include <QVariant>

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
    void resizeEvent(QResizeEvent *event);

    SettingsManager *_settingsManager;

private:
    Ui::MainWindow *ui;

    QDialog *_settingsDialog;
    bool _settingsShown;

    QDialog *_helpDialog;
    bool _helpShown;

    SlideShow* _slideshow;

    void loadSettings(void);
    void saveSettings(void);

public slots:
    void folderStatusChanged(bool exists, bool readable);
    void loadImage(const QPixmap *image);
    void displayPath(QString path);
    void on_pauseButton_clicked(void);

    void settingsClosed(void);
    void helpClosed(void);

private slots:
    void on_nextButton_clicked(void);
    void on_previousButton_clicked(void);

    void on_settingsButton_clicked();

    void on_helpButton_clicked();

signals:
    void pausePressed(void);
    void nextImageClicked(void);
    void previousImageClicked(void);
};

#endif // MAINWINDOW_H
