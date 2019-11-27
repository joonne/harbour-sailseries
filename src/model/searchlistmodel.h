#ifndef SEARCHLISTMODEL_H
#define SEARCHLISTMODEL_H

#include <QObject>
#include <QDebug>
#include <QQmlListProperty>

#include "seriesdata.h"
#include "../databasemanager.h"
#include "../api.h"

class SearchListModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<SeriesData> SearchModel READ getSearchModel NOTIFY searchModelChanged)

public:
    explicit SearchListModel(QObject *parent = 0, Api *api = 0, DatabaseManager *dbmanager = 0);
    ~SearchListModel();

    void populateSearchModel(QList<QVariantMap> foundSeries);

    Q_INVOKABLE void searchSeries(const QString &text);
    Q_INVOKABLE void getAll(const int &seriesId);
    Q_INVOKABLE void clearList();

    QQmlListProperty<SeriesData> getSearchModel();
    void setAddedFor(const int &seriesId, const bool &isAdded);

signals:
    void searchModelChanged();
    void searchSeriesRequested(QString);
    void getAllRequested(int);
    void checkIfAddedRequested(int, QString);
    void setLoading(bool);
    void updateModels();

public slots:
    void searchFinished(QList<QVariantMap> series);
    void seriesStored(const int &seriesId);
    void checkIfAddedReady(const int &seriesId, const bool &isAdded);

private:
    QList<SeriesData*> m_searchListModel;
    Api* m_api;
    DatabaseManager* m_dbmanager;

    static int searchListCount(QQmlListProperty<SeriesData> *prop);
    static SeriesData* searchListAt(QQmlListProperty<SeriesData> *prop, int index);
    static void searchListAppend(QQmlListProperty<SeriesData>* prop, SeriesData* val);
    static void searchListClear(QQmlListProperty<SeriesData>* prop);
};

#endif // SEARCHLISTMODEL_H
