#-------------------------------------------------
#
# Project created by QtCreator 2019-03-03T10:33:05
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ExamServer
TEMPLATE = app


SOURCES += main.cpp\
        monitormain.cpp \
    server.cpp \
    ../ExamCommon/client.cpp \
    ../ExamCommon/command.cpp \
    ../ExamCommon/configloader.cpp \
    ../ExamCommon/examproblem.cpp \
    datafileload.cpp \
    dataconfig.cpp \
    ../ExamCommon/examinfodao.cpp \
    ../ExamCommon/studentinfodao.cpp \
    orm.cpp \
    ../ExamCommon/classinfodao.cpp \
    ../ExamCommon/dao.cpp

HEADERS  += monitormain.h \
    server.h \
    ../ExamCommon/client.h \
    ../ExamCommon/command.h \
    ../ExamCommon/configloader.h \
    ../ExamCommon/examproblem.h \
    datafileload.h \
    dataconfig.h \
    ../ExamCommon/examinfodao.h \
    ../ExamCommon/studentinfodao.h \
    orm.h \
    ../ExamCommon/classinfodao.h \
    ../ExamCommon/dao.h \
    ../ExamCommon/clientstatus.h

FORMS    += monitormain.ui \
    dataconfig.ui

DISTFILES += \
    ../ExamCommon/ExamCommon.pri

RESOURCES += \
    r.qrc
