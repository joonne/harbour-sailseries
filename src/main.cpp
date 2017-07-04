#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>
#include <QScopedPointer>
#include <QQuickView>
#include <QQmlEngine>
#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlComponent>
#include <QTranslator>
#include <QDebug>
#include <QSettings>

#include "engine.h"
#include "settings.h"
#include "statistics.h"
#include "model/seriesdata.h"
#include "model/serieslistmodel.h"
#include "model/episodedata.h"
#include "model/episodelistmodel.h"
#include "model/searchlistmodel.h"
#include "model/todaylistmodel.h"
#include "model/seasondata.h"
#include "model/seasonlistmodel.h"

int main(int argc, char *argv[])
{

    QCoreApplication::setApplicationName("harbour-sailseries");
    QCoreApplication::setOrganizationName("harbour-sailseries");

    qmlRegisterType<SeriesData>("harbour.sailseries.model", 1, 0, "SeriesData");
    qmlRegisterType<SeriesListModel>("harbour.sailseries.model", 1, 0, "SeriesListModel");
    qmlRegisterType<SearchListModel>("harbour.sailseries.model", 1, 0, "SearchListModel");
    qmlRegisterType<TodayListModel>("harbour.sailseries.model", 1, 0, "TodayListModel");
    qmlRegisterType<Engine>("harbour.sailseries.model", 1, 0, "engine");
    qmlRegisterType<EpisodeData>("harbour.sailseries.model", 1, 0, "EpisodeData");
    qmlRegisterType<EpisodeListModel>("harbour.sailseries.model", 1, 0, "EpisodeListModel");
    qmlRegisterType<SeasonData>("harbour.sailseries.model", 1, 0, "SeasonData");
    qmlRegisterType<SeasonListModel>("harbour.sailseries.model", 1, 0, "SeasonListModel");
    qmlRegisterType<Statistics>("harbour.sailseries.model", 1, 0, "Statistics");

    // For this example, wizard-generates single line code would be good enough,
    // but very soon it won't be enough for you anyway, so use this more detailed example from start
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    // That is how you can access version strings in C++. And pass them on to QML
    view->rootContext()->setContextProperty("appVersion", APP_VERSION);
    view->rootContext()->setContextProperty("appBuildNum", APP_BUILDNUM);

    // This is the public QML datacontroller
    QScopedPointer<Engine> engine(new Engine);
    QQmlContext* context = view->rootContext();
    context->setContextProperty("engine", engine.data());

    Settings settings;
    view->rootContext()->setContextProperty("settings", &settings);

    view->engine()->addImportPath(SailfishApp::pathTo("qml/components").toString());
    view->setSource(SailfishApp::pathTo("qml/main.qml"));

    view->showFullScreen();

    return app->exec();
}
