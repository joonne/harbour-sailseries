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

class XMLReader : public QObject
{
    Q_OBJECT
public:
    explicit XMLReader(QObject *parent = 0);
    ~XMLReader();

    void parseXML(QXmlStreamReader &xml);
    QMap<QString,QString> parseSeries(QXmlStreamReader& xml);
    QMap<QString,QString> parseLanguages(QXmlStreamReader& xml);
    void addElementDataToMap(QXmlStreamReader& xml,
                                 QMap<QString, QString>& map) const;
    void startRequest(QUrl url);

    void getLanguages();
    void searchSeries(QString text);
    void fetchBanner(QString text);
    void getServerTime();

    QList<QMap<QString,QString> > getSeries();

signals:
    void readyToPopulate();

public slots:
    void replyFinished(QNetworkReply* reply);
    void sslErrors(QNetworkReply* reply, QList<QSslError>& errors);
    void bannerFetchFinished(QNetworkReply* reply);

private:

    QNetworkAccessManager* myNetWorkAccessManager;
    const QString myApiKey;
    const QString myMirrorPath;
    QList<QMap<QString,QString> > myLanguages;
    QList<QMap<QString,QString> > mySeries;

};

#endif // XMLREADER_H
