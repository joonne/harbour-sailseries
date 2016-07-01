#ifndef SEASONDATA_H
#define SEASONDATA_H

#include <QObject>

class SeasonData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int SeasonNumber READ getSeasonNumber NOTIFY seasonNumberChanged)
    Q_PROPERTY(QString SeasonBanner READ getSeasonBanner NOTIFY seasonBannerChanged)
    Q_PROPERTY(int WatchedCount READ getWatchedCount NOTIFY watchedCountChanged)
    Q_PROPERTY(int TotalCount READ getTotalCount NOTIFY totalCountChanged)
public:
    explicit SeasonData(QObject *parent = 0, int seasonNumber = 0, QString seasonBanner = "", int watchedCount = 0, int totalCount = 0);
    ~SeasonData();

    int getSeasonNumber();
    QString getSeasonBanner();
    int getWatchedCount();
    int getTotalCount();

signals:
    void seasonNumberChanged();
    void seasonBannerChanged();
    void watchedCountChanged();
    void totalCountChanged();

private:
    int m_seasonNumber;
    QString m_seasonBanner;
    int m_watchedCount;
    int m_totalCount;

};

#endif // SEASONDATA_H
