#include "settingsmanager.h"

SettingsManager::SettingsManager(QObject *parent) : QObject(parent)
{

}

bool SettingsManager::empty(void)
{
    QSettings settings(ORGANIZATION, APPLICATION);
    return (settings.allKeys().size() == 0);
}

void SettingsManager::writeSetting(const QString &key, const QVariant &variant)
{
    QSettings settings(ORGANIZATION, APPLICATION);
    settings.setValue(key, variant);
}

QVariant SettingsManager::readSetting(const QString &key)
{
    QSettings settings(ORGANIZATION, APPLICATION);
    QVariant value = settings.value(key);

    if (value.isNull()) {
        // default values
        if (key == SETTING_PATHS) {
            QStringList dirs;
            dirs.append(QDir::homePath());
            value = QVariant(dirs);
        } else if (key == SETTING_SPEED) {
            value = QVariant(2);
        } else if (key == SETTING_ON_CLICK_ACTION) {
            value = QVariant(SETTING_ON_CLICK_ACTION_OPEN_FOLDER);
        } else if (key == SETTING_LANGUAGE) {
            QString defaultLocale = QLocale::system().name();       // e.g. de_DE
            defaultLocale.truncate(defaultLocale.lastIndexOf("_")); // e.g. de
            value = QVariant(defaultLocale);
        }
    }

    writeSetting(key, value);
    return value;
}
