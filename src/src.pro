TEMPLATE=app
TARGET = harbour-sailseries

QT += network sql
LIBS +=-lz
CONFIG += sailfishapp
CONFIG += c++14

SOURCES += main.cpp \
    model/seriesdata.cpp \
    model/serieslistmodel.cpp \
    databasemanager.cpp \
    model/episodedata.cpp \
    model/episodelistmodel.cpp \
    model/searchlistmodel.cpp \
    model/todaylistmodel.cpp \
    model/seasondata.cpp \
    model/seasonlistmodel.cpp \
    engine.cpp \
    statistics.cpp \
    api.cpp

DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_BUILDNUM=\\\"$$RELEASE\\\"

message($$DEFINES)

TVDB_API_KEY = $$getenv(TVDB_API_KEY)
isEmpty(TVDB_API_KEY): error("Required environment variable TVDB_API_KEY is not set. Build aborted.")
DEFINES += TVDB_API_KEY=\\\"$$TVDB_API_KEY\\\"

message($$DEFINES)

OTHER_FILES = \
    ../rpm/harbour-sailseries.spec \
    qml/main.qml \
    qml/pages/EpisodePage.qml \
    qml/pages/SearchResultPage.qml \
    qml/pages/StartPage.qml \
    qml/pages/SearchPage.qml \
    qml/pages/MySeriesPage.qml \
    qml/pages/CoverPage.qml \
    qml/cover/CoverPage.qml \
    qml/pages/AboutPage.qml \
    qml/components/MoreIndicator.qml \
    qml/pages/EpisodesPage.qml

INCLUDEPATH += $$PWD

HEADERS += \
    model/seriesdata.h \
    model/serieslistmodel.h \
    databasemanager.h \
    model/episodedata.h \
    model/episodelistmodel.h \
    model/searchlistmodel.h \
    model/todaylistmodel.h \
    model/seasondata.h \
    model/seasonlistmodel.h \
    engine.h \
    statistics.h \
    api.h

DISTFILES += \
    qml/pages/SeasonsPage.qml \
    qml/pages/SeriesPage.qml \
    qml/pages/StatisticsPage.qml \
    qml/components/ProgressCircle.qml \
    qml/js/http.js \
    qml/js/promise.js \
    qml/js/timeout.js \
    qml/js/tvApi.js

RESOURCES += \
    harbour-sailseries.qrc
