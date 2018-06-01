#include "mainwindow.h"
#include "settingsmanager.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile styleFile(":style/dark");
    if (styleFile.open(QFile::ReadOnly)) {
          //a.setStyleSheet(styleFile.readAll());
    }

    int id = QFontDatabase::addApplicationFont(":/font/roboto");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont roboto12(family, 12);
    //a.setFont(roboto12);

    MainWindow w;
    w.show();

    if (QCoreApplication::arguments().length() >= 2 && QCoreApplication::arguments().at(1) == "--kiosk") {
      qWarning() << "Starting in kiosk mode";
      w.setWindowTitle("EasySlideshow - KIOSK");
      QScreen *screen = QGuiApplication::primaryScreen();
      w.setGeometry(screen->geometry());
    }

    return a.exec();
}
