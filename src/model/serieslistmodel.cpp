#include "serieslistmodel.h"

seriesListModel::seriesListModel(QObject *parent, QQmlContext* context) :
    QObject(parent),
    myReader(new XMLReader),
    myContext(context){



    isPopulated = false;

    qDebug() << "connecting signal and slot";
    connect(myReader,
            SIGNAL(readyToPopulate()),
            this,
            SLOT(xmlParseFinished()));

}

seriesListModel::~seriesListModel() {
    //delete mySeriesListModel;
}

void seriesListModel::xmlParseFinished() {

    qDebug("slotti toimii");
    mySeries = myReader->getSeries();
    populateSeriesList();
}

QQmlListProperty<SeriesData> seriesListModel::getSeriesList() {

    return QQmlListProperty<SeriesData>(this,&mySeriesListModel,&seriesListModel::seriesListCount,&seriesListModel::seriesListAt);

}

void seriesListModel::seriesListAppend(QQmlListProperty<SeriesData>* prop, SeriesData* val)
{
    seriesListModel* seriesModel = qobject_cast<seriesListModel*>(prop->object);
    seriesModel->mySeriesListModel.append(val);
}

SeriesData* seriesListModel::seriesListAt(QQmlListProperty<SeriesData>* prop, int index)
{
    return (qobject_cast<seriesListModel*>(prop->object))->mySeriesListModel.at(index);
}

int seriesListModel::seriesListCount(QQmlListProperty<SeriesData>* prop)
{
    return qobject_cast<seriesListModel*>(prop->object)->mySeriesListModel.size();
}

void seriesListModel::seriesListClear(QQmlListProperty<SeriesData>* prop)
{
    qobject_cast<seriesListModel*>(prop->object)->mySeriesListModel.clear();
}

// -------------------------------------------------------------------
// POPULATING DATA

void seriesListModel::populateSeriesList() {


    if(mySeries.size() != 0) {

        qDebug("getSeries works.");

        for(int i = 0; i < mySeries.size(); ++i) {

            qDebug() << mySeries.size();

            qDebug("indeksointi alkaa");
            QMap<QString,QString> temp = mySeries.at(i);
            qDebug("indeksointi loppuu");
            QString seriesid = temp["seriesid"];
            qDebug("seriesid");
            QString language = temp["langugage"];
            qDebug("language");
            QString seriesName = temp["SeriesName"];
            qDebug("seriesName");
            QString aliasNames = temp["AliasNames"];
            qDebug("aliasNames");
            QString banner = temp["banner"];
            qDebug() << banner;
            qDebug("banner");
            QString overview = temp["Overview"];
            qDebug("overview");
            QString firstAired = temp["FirstAired"];
            qDebug("firstAired");
            QString imdb_id = temp["IMDB_ID"];
            qDebug("imdb_id");
            QString zap2it_id = temp["zap2it_id"];
            qDebug("zap2it_id");
            QString network = temp["Network"];
            qDebug("network");

            qDebug("found series");
            qDebug("appending stuff to model");

            SeriesData* serie = new SeriesData(this, seriesid, language,
                                               seriesName, aliasNames,
                                               banner, overview, firstAired,
                                               imdb_id, zap2it_id, network);


            mySeriesListModel.append(serie);

        }


        // must remember to call signal to let QML side know about populated items..
        emit seriesListChanged();

        // mark flag
        isPopulated = true;

    }
}

void seriesListModel::searchSeries(QString text) {

    mySeriesListModel.clear();
    mySeries.clear();
    myReader->searchSeries(text);
}

void seriesListModel::selectSeries(int index) {

    myInfo = mySeriesListModel.at(index);
    qDebug() << "myInfo seriesname: " << myInfo->getSeriesName();

}

QString seriesListModel::getID() { return myInfo->getID(); }

QString seriesListModel::getLanguage() { return myInfo->getLanguage(); }

QString seriesListModel::getSeriesName() { return myInfo->getSeriesName(); }

QString seriesListModel::getAliasNames() { return myInfo->getAliasNames(); }

QString seriesListModel::getBanner() { return myInfo->getBanner(); }

QString seriesListModel::getOverview() { return myInfo->getOverview(); }

QString seriesListModel::getFirstAired() { return myInfo->getFirstAired(); }

QString seriesListModel::getIMDB_ID() { return myInfo->getIMDB_ID(); }

QString seriesListModel::getZap2it_ID() { return myInfo->getZap2it_ID(); }

QString seriesListModel::getNetwork() { return myInfo->getNetwork(); }





