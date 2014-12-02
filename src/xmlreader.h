#ifndef XMLREADER_H
#define XMLREADER_H

#include <QObject>
#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTemporaryFile>
#include <QList>
#include <QImage>
#include <QStringRef>
#include <QPixmap>

class XMLReader : public QObject
{
    Q_OBJECT
public:
    explicit XMLReader(QObject *parent = 0);
    ~XMLReader();

    void parseXML(QXmlStreamReader &xml);
    QMap<QString,QString> parseSeries(QXmlStreamReader& xml);
    QMap<QString,QString> parseLanguages(QXmlStreamReader& xml);
    QMap<QString,QString> parseEpisode(QXmlStreamReader& xml);
    QMap<QString, QString> parseTVChannel(QXmlStreamReader& xml);
    void addElementDataToMap(QXmlStreamReader& xml,
                                 QMap<QString, QString>& map) const;
    void startRequest(QUrl url);

    void getLanguages();
    void searchSeries(QString text);
    void getServerTime();
    void getFullSeriesRecord(QString seriesid);


    void updateTVGuide();

    QList<QMap<QString,QString> > getSeries();
    QList<QMap<QString,QString> > getEpisodes();
    QPixmap *getBanner();
    QList<QMap<QString,QList<QMap<QString,QString> > > > getTVGuide();

signals:
    void readyToPopulateSeries();
    void readyToPopulateChannels();
    void readyToFetchBanner(QString banner);
    void readyToGetBanner();
    void readyToStoreSeries();

public slots:
    void replyFinished(QNetworkReply* reply);
    void sslErrors(QNetworkReply* reply, QList<QSslError>& errors);
    void bannerFetchFinished(QNetworkReply* reply);
    void fetchBanner(QString banner);

private:

    QString selectChannel(QString text);

    QNetworkAccessManager* myNetWorkAccessManager;
    const QString myApiKey;
    const QString myMirrorPath;
    const QString tv1;
    const QString tv2;
    const QString mtv3;
    const QString nelonen;
    const QString sub;
    const QString teema;
    const QString fem;
    const QString jim;
    const QString liv;
    const QString ava;
    const QString tvviisi;
    const QString kutonen;
    const QString fox;
    const QString elokuvat;
    QList<QMap<QString,QString> > myLanguages;
    QList<QMap<QString,QString> > mySeries;
    QList<QMap<QString,QString> > myEpisodes;
    QPixmap* myBanner;
    QList<QMap<QString,QList<QMap<QString,QString> > > > myTVGuide;
    QString currentTVchannel;

    bool fullRecord;

};

#endif // XMLREADER_H
