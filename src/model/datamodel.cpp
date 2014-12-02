#include "datamodel.h"

DataModel::DataModel(QObject *parent) :
    QObject(parent),
    mySeriesListModel(new seriesListModel),
    myReader(new XMLReader)
{

    connect(myReader,
            SIGNAL(readyToPopulateChannels()),
            this,
            SLOT(xmlParseFinished()));

    myReader->updateTVGuide();

}

void DataModel::setDesiredChannel(QString channelName) {

    ProgramListModel* channel = new ProgramListModel(this);

    for(int i = 0; i < myPrograms.size(); ++i) {
        QMap<QString,QList<QMap<QString,QString> > >::const_iterator itr = myPrograms.at(i).begin();
        while(itr!=myPrograms.at(i).end()) {

            if(itr.key() == channelName) {
                QList<QMap<QString,QString> > programs = itr.value();
                channel->populatePrograms(programs);
                myChannels.append(channel);

                qDebug() << "kanava " << channel->getChannel() << " lisätty.";

                myProgramListModel = myChannels.back();
                emit programListModelChanged();
            }

            ++itr;
        }
    }
}

ProgramListModel* DataModel::getProgramListModel() { return myProgramListModel; }

seriesListModel* DataModel::getSeriesListModel() { return mySeriesListModel; }

void DataModel::xmlParseFinished() {

    myPrograms = myReader->getTVGuide();

    qDebug("tv-guiden slotti toimii");

}
