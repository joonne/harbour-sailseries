#ifndef SEARCHLISTMODEL_H
#define SEARCHLISTMODEL_H

#include <QObject>
#include <QDebug>
#include <QQmlListProperty>

#include "seriesdata.h"
#include "../databasemanager.h"
#include "../xmlreader.h"

class SearchListModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<SeriesData> SearchModel READ getSearchModel NOTIFY searchModelChanged)
    Q_PROPERTY(QString ID READ getID)
    Q_PROPERTY(QString Language READ getLanguage)
    Q_PROPERTY(QString SeriesName READ getSeriesName)
    Q_PROPERTY(QString AliasNames READ getAliasNames)
    Q_PROPERTY(QString Banner READ getBanner)
    Q_PROPERTY(QString Overview READ getOverview)
    Q_PROPERTY(QString FirstAired READ getFirstAired)
    Q_PROPERTY(QString IMDB_ID READ getIMDB_ID)
    Q_PROPERTY(QString zap2it_ID READ getZap2it_ID)
    Q_PROPERTY(QString Network READ getNetwork)
    Q_PROPERTY(bool Loading READ getLoading WRITE setLoading NOTIFY loadingChanged)
    Q_PROPERTY(bool Added READ getAdded WRITE setAdded NOTIFY addedChanged)
public:
    explicit SearchListModel(QObject *parent = 0, DatabaseManager* dbmanager = 0, XMLReader* xmlreader = 0 );
    ~SearchListModel();

    void populateSearchModel();

    Q_INVOKABLE void searchSeries(QString text);
    Q_INVOKABLE void selectSeries(int index);
    Q_INVOKABLE void getFullSeriesRecord(QString id);
    Q_INVOKABLE void clearList();
    Q_INVOKABLE void checkIfAdded(QString id, QString name);

    QQmlListProperty<SeriesData> getSearchModel();
    QString getID();
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
    void setLoading(bool);
    bool getAdded();
    void setAdded(bool cond);

    void storeSeries();
    void storeEpisodes();
    void storeBanners();

signals:
    void searchModelChanged();
    void loadingChanged();
    void addedChanged();
    void updateModels();

public slots:
    void xmlParseFinished();
    void getFullSeriesRecordFinished();

private:
    XMLReader* m_reader;
    DatabaseManager* m_dbmanager;
    QList<QMap<QString,QString> > m_series;
    QList<QMap<QString,QString> > m_episodes;
    QList<QMap<QString,QString> > m_banners;
    QList<SeriesData*> m_searchListModel;
    SeriesData* m_info;

    static int searchListCount(QQmlListProperty<SeriesData> *prop);
    static SeriesData* searchListAt(QQmlListProperty<SeriesData> *prop, int index);
    static void searchListAppend(QQmlListProperty<SeriesData>* prop, SeriesData* val);
    static void searchListClear(QQmlListProperty<SeriesData>* prop);

    bool m_loading;
    bool m_added;

    QList<QString> m_posters;
    int m_posterIndex;

    QString mode;

};

#endif // SEARCHLISTMODEL_H
