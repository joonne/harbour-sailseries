#ifndef PROGRAMDATA_H
#define PROGRAMDATA_H

#include <QObject>

class ProgramData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ProgramName READ getProgramName)
    Q_PROPERTY(QString Weekday READ getWeekday)
    Q_PROPERTY(QString Time READ getTime)
    Q_PROPERTY(QString ChannelName READ getChannelName)
    Q_PROPERTY(QString Overview READ getOverview)
public:
    explicit ProgramData(QObject *parent = 0);
    ProgramData(QObject *parent, QString programName, QString weekday,
                QString time, QString channelName, QString overview);

    QString getProgramName();
    QString getWeekday();
    QString getTime();
    QString getChannelName();
    QString getOverview();

signals:

public slots:

private:
    QString myProgramName;
    QString myWeekday;
    QString myTime;
    QString myChannelName;
    QString myOverview;

};

#endif // PROGRAMDATA_H
