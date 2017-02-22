#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QSettings>
#include <QVariant>
#include <QDebug>
#include <QDir>
#include <QStringList>
#include <QSize>
#include <QPoint>

#include "globals.h"

class SettingsManager : public QObject
{
    Q_OBJECT
public:
    static void writeSetting(const QString &key, const QVariant &variant);
    static QVariant readSetting(const QString &key);
    static bool empty(void);

private:


signals:


public slots:

};

#endif // SETTINGSMANAGER_H
