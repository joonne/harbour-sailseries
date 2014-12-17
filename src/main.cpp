#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>
#include <QScopedPointer>
#include <QQuickView>
#include <QQmlEngine>
#include <QGuiApplication>
#include <QNetworkAccessManager>
#include "xmlreader.h"
#include "model/seriesdata.h"
#include "model/serieslistmodel.h"
#include "model/programdata.h"
#include "model/programlistmodel.h"
#include "model/datamodel.h"
#include <QQmlContext>
#include <QQmlComponent>

int main(int argc, char *argv[])
{

    qmlRegisterType<SeriesData>("DataModel", 1, 0, "SeriesData");
    qmlRegisterType<seriesListModel>("DataModel", 1, 0, "SeriesListModel");
    qmlRegisterType<ProgramData>("DataModel", 1, 0, "ProgramData");
    qmlRegisterType<ProgramListModel>("DataModel", 1, 0, "ProgramListModel");
    qmlRegisterType<DataModel>("DataModel", 1, 0, "DataModel");


    // For this example, wizard-generates single line code would be good enough,
    // but very soon it won't be enough for you anyway, so use this more detailed example from start
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    seriesListModel* LISTMODEL = new seriesListModel();
    //QScopedPointer<QObject> LISTMODEL(seriesListModel());
    QQmlContext* context = view->rootContext();
    context->setContextProperty("LISTMODEL",LISTMODEL);

    //    Here's how you will add QML components whenever you start using them
    //    Check https://github.com/amarchen/Wikipedia for a more full example
    //    view->engine()->addImportPath(SailfishApp::pathTo("qml/components").toString());
    view->setSource(SailfishApp::pathTo("qml/main.qml"));

    view->showFullScreen();

    return app->exec();
}
