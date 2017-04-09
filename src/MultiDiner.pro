#-------------------------------------------------
#
# Project created by QtCreator 2017-03-09T12:02:00
#
#-------------------------------------------------

QT       += core gui svg
CONFIG -= console
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MultiDiner
TEMPLATE = app

CONFIG -= debug_and_release debug_and_release_target
#CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    mgexception.cpp \
    ../ThirdParty/tinyexpr-master/tinyexpr.c

HEADERS  += mainwindow.h \
    edge.h \
    multigraph.h \
    mgexception.h \
    wheelevent_forqsceneview.h \
    ../ThirdParty/tinyexpr-master/tinyexpr.h \
    vertex.h

FORMS    += mainwindow.ui


RESOURCES += resources.qrc
