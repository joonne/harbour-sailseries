TEMPLATE = app

TARGET = tst-harbour-sailseries

QT += testlib

target.path = /usr/bin

SOURCES = testepisodedata.cpp \
    main.cpp

HEADERS = testepisodedata.h

INSTALLS += target
