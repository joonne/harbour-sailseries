#include "statistics.h"

Statistics::Statistics(QObject *parent) : QObject(parent)
{

}

int Statistics::getWatchedEpisodesDuration() {
    return m_dbmanager->getWatchedEpisodesDuration();
}
