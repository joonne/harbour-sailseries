TEMPLATE=app
# The name of your app binary (and it's better if you think it is the whole app name as it's referred to many times)
# Must start with "harbour-"
TARGET = harbour-sailseries

QT += network sql
LIBS +=-lz
# In the bright future this config line will do a lot of stuff to you
CONFIG += sailfishapp

SOURCES += main.cpp \
    xmlreader.cpp \
    model/seriesdata.cpp \
    model/serieslistmodel.cpp \
    databasemanager.cpp \
    model/episodedata.cpp \
    model/programdata.cpp \
    model/programlistmodel.cpp \
    model/datamodel.cpp \
    model/episodelistmodel.cpp \
    model/searchlistmodel.cpp \
    model/todaylistmodel.cpp \
    qzip.cpp

# translations
translations.files = translations/harbour-sailseries-fi.qm
translations.path = /usr/share/$${TARGET}

INSTALLS += translations

TRANSLATIONS += translations/harbour-sailseries-fi.ts

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
    qml/pages/TVguidePage.qml \
    qml/pages/Channel.qml \
    qml/pages/MySeriesPage.qml \
    qml/pages/CoverPage.qml \
    qml/pages/SeriesViewPage.qml \
    qml/pages/Episodes.qml \
    qml/pages/Series.qml \
    qml/cover/CoverPage.qml \
    qml/pages/Seasons.qml \
    qml/pages/AboutPage.qml \
    qml/pages/parser.js \
    qml/components/MoreIndicator.qml \
    qml/components/TextExpander.qml \
    qml/pages/EpisodeOverviewPage_orig.qml \
    qml/pages/EpisodeOverviewPage.qml

INCLUDEPATH += $$PWD

HEADERS += \
    xmlreader.h \
    model/seriesdata.h \
    model/serieslistmodel.h \
    databasemanager.h \
    model/episodedata.h \
    model/programdata.h \
    model/programlistmodel.h \
    model/datamodel.h \
    model/episodelistmodel.h \
    model/searchlistmodel.h \
    model/todaylistmodel.h \
    qzipreader_p.h \
    qzipwriter_p.h
