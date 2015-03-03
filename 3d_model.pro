#-------------------------------------------------
#
# Project created by QtCreator 2015-02-20T10:56:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3d_model
TEMPLATE = app


SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/createcharmodeldlg.cpp \
    src/model2d_processing.cpp \
    src/model_creator.cpp \
    src/model3d.cpp \
    src/gl_widget.cpp

HEADERS  += \
    include/mainwindow.h \
    include/createcharmodeldlg.h \
    include/model2d_processing.h \
    include/model_creator.h \
    include/model3d.h \
    include/gl_widget.h

FORMS    += \
    ui/mainwindow.ui \
    ui/createcharmodeldlg.ui

RESOURCES +=
