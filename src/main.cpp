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

#include "model/seriesdata.h"
#include "model/serieslistmodel.h"
#include "model/programdata.h"
#include "model/programlistmodel.h"
#include "model/datamodel.h"
#include "model/episodedata.h"
#include "model/episodelistmodel.h"
#include "model/searchlistmodel.h"
#include "model/todaylistmodel.h"

int main(int argc, char *argv[])
{

    QCoreApplication::setApplicationName("harbour-sailseries");
    QCoreApplication::setOrganizationName("harbour-sailseries");

    qmlRegisterType<SeriesData>("harbour.sailseries.datamodel", 1, 0, "SeriesData");
    qmlRegisterType<SeriesListModel>("harbour.sailseries.datamodel", 1, 0, "SeriesListModel");
    qmlRegisterType<SearchListModel>("harbour.sailseries.datamodel", 1, 0, "SearchListModel");
    qmlRegisterType<TodayListModel>("harbour.sailseries.datamodel", 1, 0, "TodayListModel");
    qmlRegisterType<ProgramData>("harbour.sailseries.datamodel", 1, 0, "ProgramData");
    qmlRegisterType<ProgramListModel>("harbour.sailseries.datamodel", 1, 0, "ProgramListModel");
    qmlRegisterType<DataModel>("harbour.sailseries.datamodel", 1, 0, "DataModel");
    qmlRegisterType<EpisodeData>("harbour.sailseries.datamodel", 1, 0, "EpisodeData");
    qmlRegisterType<EpisodeListModel>("harbour.sailseries.datamodel", 1, 0, "EpisodeListModel");

    // For this example, wizard-generates single line code would be good enough,
    // but very soon it won't be enough for you anyway, so use this more detailed example from start
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    // this is for testing other translations
    // QLocale::setDefault(QLocale(QLocale::Spain, QLocale::Spanish));

    QTranslator *appTranslator = new QTranslator;
    appTranslator->load("harbour-sailseries-" + QLocale::system().name(), SailfishApp::pathTo("translations").path());
    app->installTranslator(appTranslator);

    // This is the public QML datacontroller
    QScopedPointer<DataModel> datamodel(new DataModel);
    QQmlContext* context = view->rootContext();
    context->setContextProperty("controller",datamodel.data());

    // Here's how you will add QML components whenever you start using them
    // Check https://github.com/amarchen/Wikipedia for a more full example
    view->engine()->addImportPath(SailfishApp::pathTo("qml/components").toString());
    view->setSource(SailfishApp::pathTo("qml/main.qml"));

    view->showFullScreen();

    return app->exec();
}
