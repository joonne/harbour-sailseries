TEMPLATE=app
# The name of your app binary (and it's better if you think it is the whole app name as it's referred to many times)
# Must start with "harbour-"
TARGET = harbour-sailseries

QT += network sql
LIBS +=-lz
# In the bright future this config line will do a lot of stuff to you
CONFIG += sailfishapp
CONFIG += c++11

SOURCES += main.cpp \
    xmlreader.cpp \
    model/seriesdata.cpp \
    model/serieslistmodel.cpp \
    databasemanager.cpp \
    model/episodedata.cpp \
    model/datamodel.cpp \
    model/episodelistmodel.cpp \
    model/searchlistmodel.cpp \
    model/todaylistmodel.cpp \
    qzip.cpp \
    model/seasondata.cpp \
    model/seasonlistmodel.cpp

DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_BUILDNUM=\\\"$$RELEASE\\\"

OTHER_FILES = \
# You DO NOT want .yaml be listed here as Qt Creator's editor is completely not ready for multi package .yaml's
#
# Also Qt Creator as of Nov 2013 will anyway try to rewrite your .yaml whenever you change your .pro
# Well, you will just have to restore .yaml from version control again and again unless you figure out
# how to kill this particular Creator's plugin
#    ../rpm/harbour-sailseries.yaml \
    ../rpm/harbour-sailseries.spec \
    qml/main.qml \
    qml/pages/StartPage.qml \
    qml/pages/SeriesInfoPage.qml \
    qml/pages/SearchPage.qml \
    qml/pages/MySeriesPage.qml \
    qml/pages/CoverPage.qml \
    qml/pages/SeriesViewPage.qml \
    qml/cover/CoverPage.qml \
    qml/pages/AboutPage.qml \
    qml/components/MoreIndicator.qml \
    qml/components/TextExpander.qml \
    qml/pages/EpisodeOverviewPage.qml \
    qml/pages/EpisodesPage.qml \
    qml/pages/ProgramOverviewPage.qml

INCLUDEPATH += $$PWD

HEADERS += \
    xmlreader.h \
    model/seriesdata.h \
    model/serieslistmodel.h \
    databasemanager.h \
    model/episodedata.h \
    model/datamodel.h \
    model/episodelistmodel.h \
    model/searchlistmodel.h \
    model/todaylistmodel.h \
    qzipreader_p.h \
    qzipwriter_p.h \
    model/seasondata.h \
    model/seasonlistmodel.h

DISTFILES += \
    qml/components/HorizontalSeparator.qml \
    qml/pages/SeasonsPage.qml \
    qml/pages/SeriesPage.qml
