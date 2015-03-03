#-------------------------------------------------
#
# Project created by QtCreator 2015-02-20T10:56:51
#
#-------------------------------------------------

#-------------------------------------------------
# This sample is published as part of the blog article at www.toptal.com/blog
# Visit www.toptal.com/blog and subscribe to our newsletter to read great posts
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
