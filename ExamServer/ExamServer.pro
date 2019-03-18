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
    ../ExamCommon/classinfodao.cpp \
    ../ExamCommon/dao.cpp \
    ../ExamCommon/answerinfo.cpp \
    ../ExamCommon/orm.cpp

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
    ../ExamCommon/classinfodao.h \
    ../ExamCommon/dao.h \
    ../ExamCommon/clientstatus.h \
    ../ExamCommon/answerinfo.h \
    ../ExamCommon/orm.h

FORMS    += monitormain.ui \
    dataconfig.ui

RESOURCES += \
    r.qrc
