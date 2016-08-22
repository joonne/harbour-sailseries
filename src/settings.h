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

    Q_INVOKABLE void setNotificationPreference(bool value);
    Q_INVOKABLE bool getNotificationPreference();

    Q_INVOKABLE void setContentUpdatePreference(bool value);
    Q_INVOKABLE bool getContentUpdatePreference();

signals:

public slots:
};

#endif // SETTINGS_H
