#include "mainwindow.h"
#include "settingsmanager.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile styleFile(":style/dark");
    if (styleFile.open(QFile::ReadOnly)) {
          a.setStyleSheet(styleFile.readAll());
    }

    MainWindow w;
    w.show();

    return a.exec();
}
