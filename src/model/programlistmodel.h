#ifndef PROGRAMLISTMODEL_H
#define PROGRAMLISTMODEL_H

#include <QObject>
#include <QQmlListProperty>
#include <QMap>
#include "programdata.h"

class ProgramListModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<ProgramData> programList READ getProgramList NOTIFY programListChanged)
    Q_PROPERTY(QString channel READ getChannel)
public:
    explicit ProgramListModel(QObject *parent = 0);

    QQmlListProperty<ProgramData> getProgramList();

    Q_INVOKABLE void populatePrograms(QList<QMap<QString,QString> > programs);

    QString getChannel();

signals:
    void programListChanged();
    void readyToPopulate();

public slots:

private:

    QList<ProgramData*> myProgramListModel;
    bool isPopulated;
    QString myChannel;

    static int ProgramListCount(QQmlListProperty<ProgramData> *prop);
    static ProgramData* ProgramListAt(QQmlListProperty<ProgramData> *prop, int index);
    static void ProgramListAppend(QQmlListProperty<ProgramData>* prop, ProgramData* val);
    static void ProgramListClear(QQmlListProperty<ProgramData>* prop);




};

#endif // PROGRAMLISTMODEL_H
