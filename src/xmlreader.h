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

class XMLReader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString seriesText READ seriesText NOTIFY seriesTextChanged)
public:
    explicit XMLReader(QObject *parent = 0);
    ~XMLReader();

    void parseXML(QXmlStreamReader &xml);
    QMap<QString,QString> parseSeries(QXmlStreamReader& xml);
    QMap<QString,QString> parseLanguages(QXmlStreamReader& xml);
    void addElementDataToMap(QXmlStreamReader& xml,
                                 QMap<QString, QString>& map) const;
    void startRequest(QUrl url);

    Q_INVOKABLE void getLanguages();
    Q_INVOKABLE void searchSeries(QString text);
    void fetchBanner(QString text);

    QList<QMap<QString,QString> > getSeries();

    QString seriesText();

signals:
    void seriesTextChanged();
    void readyToPopulate();

public slots:
    void replyFinished(QNetworkReply* reply);
    void sslErrors(QNetworkReply* reply, QList<QSslError>& errors);

private:

    QNetworkAccessManager* myNetWorkAccessManager;
    const QString myApiKey;
    const QString myMirrorPath;
    QList<QMap<QString,QString> > myLanguages;
    QList<QMap<QString,QString> > mySeries;
    QString mySeriesText;

};

#endif // XMLREADER_H
