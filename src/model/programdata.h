#ifndef PROGRAMDATA_H
#define PROGRAMDATA_H

#include <QObject>

class ProgramData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString programName READ getProgramName)
    Q_PROPERTY(QString weekday READ getWeekday)
    Q_PROPERTY(QString time READ getTime)
public:
    explicit ProgramData(QObject *parent = 0);
    ProgramData(QObject *parent, QString programName, QString weekday, QString time);

    QString getProgramName();
    QString getWeekday();
    QString getTime();


signals:

public slots:

private:
    QString myProgramName;
    QString myWeekday;
    QString myTime;

};

#endif // PROGRAMDATA_H
