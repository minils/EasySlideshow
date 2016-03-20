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
        if (key == SETTING_PATH) {
            value = QVariant(QDir::homePath());
        } else if (key == SETTING_SPEED) {
            value = QVariant(2);
        } else if (key == SETTING_ON_CLICK_ACTION) {
            value = QVariant(SETTING_ON_CLICK_ACTION_OPEN_FOLDER);
        }
    }

    writeSetting(key, value);
    return value;
}
