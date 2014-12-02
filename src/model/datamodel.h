#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include "programlistmodel.h"
#include "serieslistmodel.h"
#include "../xmlreader.h"

class DataModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ProgramListModel* programListModel READ getProgramListModel NOTIFY programListModelChanged)
    Q_PROPERTY(seriesListModel* SeriesListModel READ getSeriesListModel NOTIFY seriesListModelChanged)
public:
    explicit DataModel(QObject *parent = 0);

    Q_INVOKABLE void setDesiredChannel(QString channelName);
    ProgramListModel* getProgramListModel();
    seriesListModel* getSeriesListModel();

signals:
    void seriesListModelChanged();
    void programListModelChanged();

public slots:
    void xmlParseFinished();

private:
    QList<ProgramListModel*> myChannels;
    ProgramListModel* myProgramListModel;
    seriesListModel* mySeriesListModel;
    XMLReader* myReader;
    QList<QMap<QString,QList<QMap<QString,QString> > > > myPrograms;


};

#endif // DATAMODEL_H
