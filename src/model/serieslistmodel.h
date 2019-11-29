#ifndef SERIESLISTMODEL_H
#define SERIESLISTMODEL_H

#include <QObject>
#include <QDebug>
#include <QQmlListProperty>

#include "seriesdata.h"
#include "episodedata.h"
#include "../api.h"
#include "../databasemanager.h"

class SeriesListModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<SeriesData> SeriesList READ getSeriesList NOTIFY seriesListChanged)
    Q_PROPERTY(QString Mode READ getMode WRITE setMode NOTIFY modeChanged)

public:
    explicit SeriesListModel(QObject *parent = 0, Api *api = 0, DatabaseManager *dbmanager = 0);
    ~SeriesListModel();

    Q_INVOKABLE void populate();
    Q_INVOKABLE void selectSeries(const int &index);
    Q_INVOKABLE void deleteSeries(const int &seriesId);
    Q_INVOKABLE void updateSeries(const int &seriesId);
    Q_INVOKABLE void updateAllSeries(const bool &includeEndedSeries = true);

    QQmlListProperty<SeriesData> getSeriesList();
    QString getMode();
    void setMode(QString mode);

signals:
    void seriesListChanged();
    void posterChanged(const QString &aPoster);
    void modeChanged();
    void updateModels();

    void getSeries();
    void deleteSeriesWith(int);
    void getAllRequested(int);
    void getSeriesIds(bool);
    void setLoading(bool);

public slots:
    void populateBannerList(const QList<QVariantMap> &allSeries);
    void seriesDeleted();

private:
    Api* m_api;
    DatabaseManager* m_dbmanager;
    QList<SeriesData*> m_seriesListModel;
    QString m_mode;

    static int seriesListCount(QQmlListProperty<SeriesData> *prop);
    static SeriesData* seriesListAt(QQmlListProperty<SeriesData> *prop, int index);
    static void seriesListAppend(QQmlListProperty<SeriesData>* prop, SeriesData* val);
    static void seriesListClear(QQmlListProperty<SeriesData>* prop);
};

#endif // SERIESLISTMODEL_H
