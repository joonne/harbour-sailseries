#include "xmlreader.h"

XMLReader::XMLReader(QObject *parent) :
    QObject(parent),
    myNetWorkAccessManager(0),
    myApiKey("88D0BD893851FA78"),
    myMirrorPath("http://thetvdb.com")
{
    myNetWorkAccessManager = new QNetworkAccessManager(this);

    connect(myNetWorkAccessManager,
            SIGNAL(finished(QNetworkReply*)),
            this,
            SLOT(replyFinished(QNetworkReply*)));

    connect(myNetWorkAccessManager,
            SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>&)),
            this,
            SLOT(sslErrors(QNetworkReply*,QList<QSslError>&)));
}

XMLReader::~XMLReader() {

}

void XMLReader::getLanguages() {

    QString url = myMirrorPath + "/api/" + myApiKey + "/languages.xml";
    qDebug() << "Requesting" << url;
    QUrl finalUrl(url);
    startRequest(finalUrl);
}

void XMLReader::searchSeries(QString text) {

    //mySeries.clear();
    QString url = myMirrorPath + "/api/GetSeries.php?seriesname=" + text;
    qDebug() << "Requesting" << url;
    QUrl finalUrl(url);
    startRequest(finalUrl);
}

void XMLReader::fetchBanner(QString text) {

    QString url = myMirrorPath + "/" + text;
    qDebug() << "Requesting" << url;
    QUrl finalUrl(url);
    startRequest(finalUrl);
}

QList<QMap<QString,QString> > XMLReader::getSeries() {

    qDebug("getSeries()");

    if(mySeries.size() > 0) {
        qDebug() << mySeries.at(0).find("Overview").value();
    }
    return mySeries;
}

void XMLReader::startRequest(QUrl url) {

    QNetworkRequest request(url);
    myNetWorkAccessManager->get(request);

}

// ---------------------------------------------------
// slots
// ---------------------------------------------------

void XMLReader::replyFinished(QNetworkReply *reply) {

    QString data = QString(reply->readAll());
    QXmlStreamReader xml(data);
    qDebug() << "Starting to parse xml.";
    parseXML(xml);

    reply->deleteLater();
}

void XMLReader::sslErrors(QNetworkReply *reply, QList<QSslError> &errors) {


}

// ---------------------------------------------------

void XMLReader::parseXML(QXmlStreamReader& xml) {

    QList< QMap<QString,QString> > series;
    QList< QMap<QString,QString> > languages;

    /* We'll parse the XML until we reach end of it.*/
    while(!xml.atEnd() &&
          !xml.hasError()) {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = xml.readNext();
        /* If token is just StartDocument, we'll go to next.*/
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }
        /* If token is StartElement, we'll see if we can read it.*/
        if(token == QXmlStreamReader::StartElement) {
            /* If it's named Data, we'll go to the next.*/
            if(xml.name() == "Data") {
                continue;
            }
            /* If it's named Languages, we'll go to the next.*/
            if(xml.name() == "Languages") {
                continue;
            }
            /* If it's named Series, we'll dig the information from there.*/
            if(xml.name() == "Series") {
                series.append(this->parseSeries(xml));
            }
            if(xml.name() == "Language") {
                languages.append(this->parseLanguages(xml));
            }
        }
    }
    /* Error handling. */
    if(xml.hasError()) {
        qDebug() << xml.errorString();
    }
    /* Removes any device() or data from the reader
         * and resets its internal state to the initial state. */
    xml.clear();
    myLanguages = languages;

    mySeries = series;
    qDebug() << "emitting readyToPopulate now";
    emit readyToPopulate();
}

QMap<QString, QString> XMLReader::parseSeries(QXmlStreamReader &xml) {

    QMap<QString, QString> series;
    /* Let's check that we're really getting a series. */
    if(xml.tokenType() != QXmlStreamReader::StartElement &&
            xml.name() == "Series") {
        return series;
    }
    /* Let's get the attributes for series */
    QXmlStreamAttributes attributes = xml.attributes();
    /* Let's check that series has id attribute. */
    if(attributes.hasAttribute("seriesid")) {
        /* We'll add it to the map. */
        series["seriesid"] = attributes.value("seriesid").toString();
    }
    /* Next element... */
    xml.readNext();
    /*
        * We're going to loop over the things because the order might change.
        * We'll continue the loop until we hit an EndElement named Series.
        */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "Series")) {

        if(xml.tokenType() == QXmlStreamReader::StartElement) {

            /* We've found language. */
            if(xml.name() == "language") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found SeriesName. */
            if(xml.name() == "SeriesName") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found AliasNames. */
            if(xml.name() == "AliasNames") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found banner. */
            if(xml.name() == "banner") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found Overview. */
            if(xml.name() == "Overview") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found FirstAired. */
            if(xml.name() == "FirstAired") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found IMDB_ID. */
            if(xml.name() == "IMDB_ID") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found zap2it_id. */
            if(xml.name() == "zap2it_id") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found Network. */
            if(xml.name() == "Network") {
                this->addElementDataToMap(xml, series);
            }
        }
        /* ...and next... */
        xml.readNext();
    }
    return series;
}

QMap<QString,QString> XMLReader::parseLanguages(QXmlStreamReader &xml) {


    QMap<QString, QString> languages;
    /* Let's check that we're really getting a language. */
    if(xml.tokenType() != QXmlStreamReader::StartElement &&
            xml.name() == "Languages") {
        return languages;
    }
    /* Let's get the attributes for language */
    QXmlStreamAttributes attributes = xml.attributes();
    /* Let's check that person has id attribute. */
    if(attributes.hasAttribute("id")) {
        /* We'll add it to the map. */
        languages["id"] = attributes.value("id").toString();
    }
    /* Next element... */
    xml.readNext();
    /*
        * We're going to loop over the things because the order might change.
        * We'll continue the loop until we hit an EndElement named Language.
        */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "Language")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found name. */
            if(xml.name() == "name") {
                this->addElementDataToMap(xml, languages);
            }
            /* We've found abbreviation. */
            if(xml.name() == "abbreviation") {
                this->addElementDataToMap(xml, languages);
            }
        }
        /* ...and next... */
        xml.readNext();
    }
    return languages;
}

void XMLReader::addElementDataToMap(QXmlStreamReader& xml,
                                    QMap<QString, QString>& map) const {
    /* We need a start element, like <foo> */
    if(xml.tokenType() != QXmlStreamReader::StartElement) {
        return;
    }
    /* Let's read the name... */
    QString elementName = xml.name().toString();
    /* ...go to the next. */
    xml.readNext();
    /*
     * This elements needs to contain Characters so we know it's
     * actually data, if it's not we'll leave.
     */
    if(xml.tokenType() != QXmlStreamReader::Characters) {
        return;
    }
    /* Now we can add it to the map.*/
    map.insert(elementName, xml.text().toString());
}

QString XMLReader::seriesText() {


    if(mySeries.size() == 0) {

        return "not found!";

    } else {

        QMap<QString, QString> temp = mySeries.at(0);
        QString text = temp.find("Overview").value();
        qDebug() << text;
        return text;

    }
}
