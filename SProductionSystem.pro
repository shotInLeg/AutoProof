#-------------------------------------------------
#
# Project created by QtCreator 2016-12-22T02:30:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SProductionSystem
TEMPLATE = app


SOURCES += main.cpp\
        sproductionsystem.cpp \
    SProductionKernel/sproductionproccessor.cpp \
    SProductionKernel/sproductionobjects.cpp \
    SProductionKernel/sproductionlinks.cpp

HEADERS  += sproductionsystem.h \
    SProductionKernel/sproductionproccessor.h \
    SProductionKernel/sproductionobjects.h \
    SProductionKernel/sproductionlinks.h

FORMS    += sproductionsystem.ui
