#-------------------------------------------------
#
# Project created by QtCreator 2019-02-18T12:41:34
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Exam
TEMPLATE = app


SOURCES += main.cpp\
    settingdialog.cpp \
    studentmain.cpp \
    initform.cpp \
    ../ExamCommon/client.cpp \
    ../ExamCommon/command.cpp \
    ../ExamCommon/configloader.cpp \
    ../ExamCommon/examproblem.cpp \
    ../ExamCommon/answerinfo.cpp \
    ../ExamCommon/classinfodao.cpp \
    ../ExamCommon/dao.cpp \
    ../ExamCommon/examinfodao.cpp \
    ../ExamCommon/studentinfodao.cpp \
    ../ExamCommon/orm.cpp \
    localcache.cpp

HEADERS  += \
    settingdialog.h \
    studentmain.h \
    initform.h \
    ../ExamCommon/client.h \
    ../ExamCommon/command.h \
    ../ExamCommon/configloader.h \
    ../ExamCommon/examproblem.h \
    ../ExamCommon/answerinfo.h \
    ../ExamCommon/classinfodao.h \
    ../ExamCommon/clientstatus.h \
    ../ExamCommon/dao.h \
    ../ExamCommon/examinfodao.h \
    ../ExamCommon/studentinfodao.h \
    ../ExamCommon/orm.h \
    localcache.h

FORMS    += \
    settingdialog.ui \
    studentmain.ui \
    initform.ui

RESOURCES += \
    r/r.qrc
