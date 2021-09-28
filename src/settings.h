#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);
    ~Settings();

    Q_INVOKABLE void setUpdateEndedSeriesPreference(bool value);
    Q_INVOKABLE bool getUpdateEndedSeriesPreference();

signals:

public slots:
};

#endif // SETTINGS_H
