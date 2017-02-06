#-------------------------------------------------
#
# Project created by QtCreator 2016-02-29T16:40:26
#
#-------------------------------------------------

QT       += core gui

win32 {
    QTPLUGIN += qsvg
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EasySlideshow
TEMPLATE = app

VPATH += . ./src ./ui

SOURCES += main.cpp \
        mainwindow.cpp \
    slideshow.cpp \
    displaylabel.cpp \
    settingsmanager.cpp \
    settingsdialog.cpp \
    helpdialog.cpp \
    pathscanner.cpp

HEADERS  += mainwindow.h \
    slideshow.h \
    displaylabel.h \
    globals.h \
    settingsmanager.h \
    settingsdialog.h \
    helpdialog.h \
    pathscanner.h

FORMS    += mainwindow.ui \
    settingsdialog.ui \
    helpdialog.ui

RESOURCES += resources.qrc

DISTFILES +=

TRANSLATIONS += l10n/EasySlideshow_en.ts \
    l10n/EasySlideshow_de.ts

ICON = svg/icon.svg

# extras for versioning
versionTarget.target = ./version.h
versionTarget.depends = FORCE
versionTarget.commands = touch ../version.h; git log -n 1 --format=format:\'$${LITERAL_HASH}define GIT_COMMIT \"%h\"%n\' HEAD 2>/dev/null > ../version.h; true
PRE_TARGETDEPS += ./version.h
QMAKE_EXTRA_TARGETS += versionTarget
