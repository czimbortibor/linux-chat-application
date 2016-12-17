#-------------------------------------------------
#
# Project created by QtCreator 2016-11-13T23:22:32
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES +=\
    Client.cpp \
    LoginDialog.cpp \
    Main.cpp \
    MainWindow.cpp

HEADERS  += \
    Client.h \
    LoginDialog.h \
    MainWindow.h

FORMS    += mainwindow.ui \
    logindialog.ui
