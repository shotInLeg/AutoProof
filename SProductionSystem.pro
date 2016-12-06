#-------------------------------------------------
#
# Project created by QtCreator 2016-12-06T17:27:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SProductionSystem
TEMPLATE = app


SOURCES += main.cpp\
        sproductionsystem.cpp \
    SProductionKernel/sproductionkernel.cpp \
    SProductionKernel/sproductionobjects.cpp

HEADERS  += sproductionsystem.h \
    SProductionKernel/sproductionkernel.h \
    SProductionKernel/sproductionobjects.h

FORMS    += sproductionsystem.ui
