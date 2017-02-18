#ifndef TESTEPISODEDATA_H
#define TESTEPISODEDATA_H

#include <QObject>
#include <QTest>
#include <QtTest/QtTest>
#include "../src/model/episodedata.h"

class TestEpisodeData : public QObject
{
    Q_OBJECT
private slots:
    void test();
};

#endif // TESTEPISODEDATA_H
