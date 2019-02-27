#-------------------------------------------------
#
# Project created by QtCreator 2016-02-29T16:40:26
#
#-------------------------------------------------

QT       += core gui
CONFIG	 += c++11

win32 {
    QTPLUGIN += qsvg
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = easyslideshow
TEMPLATE = app

VPATH += . ./src ./ui

SOURCES += main.cpp \
        mainwindow.cpp \
    slideshow.cpp \
    displaylabel.cpp \
    settingsmanager.cpp \
    settingsdialog.cpp \
    helpdialog.cpp \
    pathscanner.cpp \
    detailsdialog.cpp \
    src/slideshowimage.cpp

HEADERS  += mainwindow.h \
    slideshow.h \
    displaylabel.h \
    globals.h \
    settingsmanager.h \
    settingsdialog.h \
    helpdialog.h \
    pathscanner.h \
    detailsdialog.h \
    src/slideshowimage.h

FORMS    += mainwindow.ui \
    settingsdialog.ui \
    helpdialog.ui \
    detailsdialog.ui

RESOURCES += resources.qrc

DISTFILES +=

LIBS += -lexif

TRANSLATIONS += l10n/EasySlideshow_en.ts \
    l10n/EasySlideshow_de.ts \
    l10n/EasySlideshow_es.ts \
    l10n/EasySlideshow_ru.ts

ICON = svg/easyslideshow.svg
win32:RC_ICONS += ico/windows_icon.ico

# extras for versioning
versionTarget.target = version.h
versionTarget.depends = FORCE
versionTarget.commands = touch ./version.h; cd $$PWD; git log -n 1 --format=format:\'$${LITERAL_HASH}define GIT_COMMIT \"%h\"%n\' HEAD 2>/dev/null > $$OUT_PWD/version.h; true
#QMAKE_EXTRA_TARGETS += versionTarget
#PRE_TARGETDEPS += version.h

# installation
target.path = /usr/bin

icon.path = /usr/share/icons/hicolor/scalable/apps
icon.files = svg/easyslideshow.svg

desktop.path = /usr/share/applications
desktop.files = desktop/easyslideshow.desktop

INSTALLS += target \
icon \
desktop
