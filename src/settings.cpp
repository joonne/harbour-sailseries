#include "settings.h"

Settings::Settings(QObject *parent) : QObject(parent) { }

Settings::~Settings() { }

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
