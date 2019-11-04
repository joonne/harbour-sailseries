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
    Q_PROPERTY(int ID READ getID NOTIFY idChanged)
    Q_PROPERTY(QString Language READ getLanguage NOTIFY languageChanged)
    Q_PROPERTY(QString SeriesName READ getSeriesName NOTIFY seriesNameChanged)
    Q_PROPERTY(QString AliasNames READ getAliasNames NOTIFY aliasNamesChanged)
    Q_PROPERTY(QString Banner READ getBanner NOTIFY bannerChanged)
    Q_PROPERTY(QString Overview READ getOverview NOTIFY overviewChanged)
    Q_PROPERTY(QString FirstAired READ getFirstAired NOTIFY firstAiredChanged)
    Q_PROPERTY(QString IMDB_ID READ getIMDB_ID NOTIFY imdbIdChanged)
    Q_PROPERTY(QString zap2it_ID READ getZap2it_ID NOTIFY zap2itIdChanged)
    Q_PROPERTY(QString Network READ getNetwork NOTIFY networkChanged)
    Q_PROPERTY(bool Loading READ getLoading WRITE setLoading NOTIFY loadingChanged)
    Q_PROPERTY(bool Added READ getAdded WRITE setAdded NOTIFY addedChanged)
public:
    explicit SearchListModel(QObject *parent = 0, Api *api = 0, DatabaseManager *dbmanager = 0);
    ~SearchListModel();

    void populateSearchModel(QList<QVariantMap> foundSeries);

    Q_INVOKABLE void searchSeries(const QString &text);
    Q_INVOKABLE void selectSeries(const int &index);
    Q_INVOKABLE void getAll(const int &seriesId);
    Q_INVOKABLE void clearList();

    QQmlListProperty<SeriesData> getSearchModel();
    int getID();
    QString getLanguage();
    QString getSeriesName();
    QString getAliasNames();
    QString getBanner();
    QString getOverview();
    QString getFirstAired();
    QString getIMDB_ID();
    QString getZap2it_ID();
    QString getNetwork();
    bool getLoading();
    void setLoading(bool state);
    bool getAdded();
    void setAdded(bool isAdded);

signals:
    void searchModelChanged();
    void idChanged();
    void languageChanged();
    void seriesNameChanged();
    void aliasNamesChanged();
    void bannerChanged();
    void overviewChanged();
    void firstAiredChanged();
    void imdbIdChanged();
    void zap2itIdChanged();
    void networkChanged();
    void loadingChanged();
    void addedChanged();
    void updateModels();
    void searchSeriesRequested(QString);
    void getAllRequested(int);
    void checkIfAddedRequested(int, QString);

public slots:
    void searchFinished(QList<QVariantMap> series);
    void seriesStored();
    void checkIfAddedReady(const bool &isAdded);

private:
    QList<SeriesData*> m_searchListModel;
    Api* m_api;
    DatabaseManager* m_dbmanager;
    SeriesData* m_info;
    bool m_loading;
    bool m_added;

    static int searchListCount(QQmlListProperty<SeriesData> *prop);
    static SeriesData* searchListAt(QQmlListProperty<SeriesData> *prop, int index);
    static void searchListAppend(QQmlListProperty<SeriesData>* prop, SeriesData* val);
    static void searchListClear(QQmlListProperty<SeriesData>* prop);
};

#endif // SEARCHLISTMODEL_H
