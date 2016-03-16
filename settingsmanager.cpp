#include "settingsmanager.h"

SettingsManager::SettingsManager(QObject *parent) : QObject(parent)
{
    if (empty()) {
        // write initial config
        writeSetting(SETTING_PATH, QVariant(QDir::homePath()));
        writeSetting(SETTING_SPEED, QVariant(2));
    }
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

QVariant SettingsManager::readSetting(const QString &key, const QVariant &defaultValue)
{
    QSettings settings(ORGANIZATION, APPLICATION);

    return settings.value(key, defaultValue);
}
