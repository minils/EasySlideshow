#-------------------------------------------------
#
# Project created by QtCreator 2016-02-29T16:40:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EasySlideshow
TEMPLATE = app


SOURCES += main.cpp \
        mainwindow.cpp \
    slideshow.cpp \
    displaylabel.cpp \
    settingsmanager.cpp \
    settingsdialog.cpp \
    helpdialog.cpp

HEADERS  += mainwindow.h \
    slideshow.h \
    displaylabel.h \
    globals.h \
    settingsmanager.h \
    settingsdialog.h \
    helpdialog.h

FORMS    += mainwindow.ui \
    settingsdialog.ui \
    helpdialog.ui

RESOURCES += resources.qrc

DISTFILES +=
