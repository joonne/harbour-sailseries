#include "settings.h"

Settings::Settings(QObject *parent) : QObject(parent) { }

Settings::~Settings() { }

void Settings::setNotificationPreference(bool value)
{
    QSettings settings("harbour-sailseries", "harbour-sailseries");
    settings.beginGroup("Settings");
    settings.setValue("notificationPreference", value);
    settings.endGroup();
}

bool Settings::getNotificationPreference()
{
    QSettings settings("harbour-sailseries", "harbour-sailseries");
    settings.beginGroup("Settings");
    bool notificationPreference = settings.value("notificationPreference", false).toBool();
    settings.endGroup();
    return notificationPreference;
}

void Settings::setContentUpdatePreference(bool value)
{
    QSettings settings("harbour-sailseries", "harbour-sailseries");
    settings.beginGroup("Settings");
    settings.setValue("contentUpdatePreference", value);
    settings.endGroup();
}

bool Settings::getContentUpdatePreference()
{
    QSettings settings("harbour-sailseries", "harbour-sailseries");
    settings.beginGroup("Settings");
    bool contentUpdatePreference = settings.value("contentUpdatePreference", false).toBool();
    settings.endGroup();
    return contentUpdatePreference;
}

void Settings::setUpdateEndedSeriesPreference(bool value)
{
    QSettings settings("harbour-sailseries", "harbour-sailseries");
    settings.beginGroup("Settings");
    settings.setValue("updateEndedSeriesPreference", value);
    settings.endGroup();
}

bool Settings::getUpdateEndedSeriesPreference()
{
    QSettings settings("harbour-sailseries", "harbour-sailseries");
    settings.beginGroup("Settings");
    bool contentUpdatePreference = settings.value("updateEndedSeriesPreference", false).toBool();
    settings.endGroup();
    return contentUpdatePreference;
}
