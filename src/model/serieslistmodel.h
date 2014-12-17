#ifndef SERIESLISTMODEL_H
#define SERIESLISTMODEL_H

#include <QObject>
#include <QQmlContext>
#include "seriesdata.h"
#include "../xmlreader.h"
#include "../databasemanager.h"
#include <QDebug>
#include <QQmlListProperty>
#include <QPixmap>
#include <QDate>

class seriesListModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<SeriesData> seriesList READ getSeriesList NOTIFY seriesListChanged)
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
    Q_PROPERTY(QString Poster READ getPoster WRITE setPoster NOTIFY posterChanged)
    Q_PROPERTY(QString Mode READ getMode WRITE setMode NOTIFY modeChanged)

public:
    explicit seriesListModel(QObject *parent = 0, QQmlContext* context = 0);
    ~seriesListModel();

    void populateSeriesList();
    Q_INVOKABLE void populateBannerList();
    Q_INVOKABLE void populateTodayList();
    Q_INVOKABLE void searchSeries(QString text);
    Q_INVOKABLE void selectSeries(int index);
    Q_INVOKABLE void getFullSeriesRecord(QString id);
    Q_INVOKABLE void nextPoster();
    Q_INVOKABLE void clearList();

    QQmlListProperty<SeriesData> getSeriesList();
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

    QString getPoster();
    void setPoster(QString);

    bool getLoading();
    void setLoading(bool);

    QString getMode();
    void setMode(QString newmode);

    void storeSeries();
    void storeEpisodes();


signals:
    void seriesListChanged();
    void loadingChanged();
    void posterChanged();
    void modeChanged();

public slots:
    void xmlParseFinished();
    void getFullSeriesRecordFinished();

private:
    XMLReader* myReader;
    QList<QMap<QString,QString> > mySeries;
    QList<QMap<QString,QString> > myEpisodes;
    QPixmap* myBanner;
    QQmlContext* myContext;
    QList<SeriesData*> mySeriesListModel;
    bool isPopulated;
    SeriesData* myInfo;
    DatabaseManager* mydbmanager;

    static int seriesListCount(QQmlListProperty<SeriesData> *prop);
    static SeriesData* seriesListAt(QQmlListProperty<SeriesData> *prop, int index);
    static void seriesListAppend(QQmlListProperty<SeriesData>* prop, SeriesData* val);
    static void seriesListClear(QQmlListProperty<SeriesData>* prop);
    bool myLoading;

    QList<QString> myPosters;
    int posterIndex;

    QString mode;



};

#endif // SERIESLISTMODEL_H
