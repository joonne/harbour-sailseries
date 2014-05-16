#include "serieslistmodel.h"

// -------------------------------------------------------------------
// LIST HANDLING METHODS (NEEDED WHEN CREATING LIST PROPERTY ON CONSTRUCTOR)

static void seriesListAppend(QQmlListProperty<SeriesData>* prop, SeriesData* val)
{
    SeriesListModelPrivate* mySeriesModel = static_cast<SeriesListModelPrivate*>(prop->data);
    mySeriesModel->seriesListData.append(val);
}

static SeriesData* seriesListAt(QQmlListProperty<SeriesData>* prop, int index)
{
    return (static_cast<SeriesListModelPrivate*>(prop->data))->seriesListData.at(index);
}

static int seriesListCount(QQmlListProperty<SeriesData>* prop)
{
    return static_cast<SeriesListModelPrivate*>(prop->data)->seriesListData.size();
}

static void seriesListClear(QQmlListProperty<SeriesData>* prop)
{
    static_cast<SeriesListModelPrivate*>(prop->data)->seriesListData.clear();
}


SeriesListModel::SeriesListModel(QObject *parent) :
    QObject(parent),
    mySeriesModel(new SeriesListModelPrivate),
    myReader(new XMLReader)
{
    mySeriesModel->isPopulated = false;

    // create list property
    mySeriesModel->seriesList
            = new QQmlListProperty<SeriesData>(this, mySeriesModel,
                                               seriesListAppend,
                                               seriesListCount,
                                               seriesListAt,
                                               seriesListClear);

    qDebug() << "connecting signal and slot";
    connect(myReader,
            SIGNAL(readyToPopulate()),
            this,
            SLOT(xmlParseFinished()));

}

SeriesListModel::~SeriesListModel() {
    delete mySeriesModel;
}

QQmlListProperty<SeriesData> SeriesListModel::seriesList() {
    return *(mySeriesModel->seriesList);
}

void SeriesListModel::xmlParseFinished() {

    qDebug("slotti toimii");
    mySeries = myReader->getSeries();
    populateSeriesList();
}

// -------------------------------------------------------------------
// POPULATING DATA

void SeriesListModel::populateSeriesList() {

    // TODO : how to synchronize?

    if(!mySeriesModel->isPopulated) {
        qDebug("Populating SeriesListModel...");

        if(mySeries.size() != 0) {
            qDebug("getSeries works.");
        } else {
            qDebug("juna meni jo");
        }

        for(int i = 0; i < mySeries.size(); ++i){

            QMap<QString,QString> temp = mySeries.at(i);
            QString seriesid = temp.find("seriesid").value();
            QString language = temp.find("langugage").value();
            QString seriesName = temp.find("SeriesName").value();
            QString aliasNames = temp.find("AliasNames").value();
            QString banner = temp.find("banner").value();
            QString overview = temp.find("Overview").value();
            QString firstAired = temp.find("FirstAired").value();
            QString imdb_id = temp.find("IMDB_ID").value();
            QString zap2it_id = temp.find("zap2it_id").value();
            QString network = temp.find("Network").value();

            qDebug("found series");
            qDebug("Starting to append stuff to model");

            mySeriesModel->seriesListData.append(new SeriesData(this, seriesid, language,
                                                                seriesName, aliasNames,
                                                                banner, firstAired,
                                                                imdb_id, zap2it_id, network));


        }

        // must remember to call signal to let QML side know about populated items..
        seriesListChanged();

        // mark flag
        //mySeriesModel->isPopulated = true;
    }
}

void SeriesListModel::searchSeries(QString text) {

    myReader->searchSeries(text);
}

