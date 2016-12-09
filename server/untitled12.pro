#-------------------------------------------------
#
# Project created by QtCreator 2015-06-10T15:09:49
#
#-------------------------------------------------

QT       += core gui
QT += network

QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled12
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    class_function.cpp

HEADERS  += widget.h \
    class.h \
    define.h \
    function.h \
    List.h \
    Id.h \
    typedef_List.h

FORMS    += widget.ui
