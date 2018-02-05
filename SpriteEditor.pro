#-------------------------------------------------
#
# Project created by QtCreator 2016-10-21T16:37:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpriteEditor
TEMPLATE = app


SOURCES += main.cpp\
        editorwindow.cpp \
    toolswidget.cpp \
    previewwidget.cpp \
    framemanagerwidget.cpp \
    layermanagerwidget.cpp \
    layerlist.cpp \
    framelist.cpp \
    sprite.cpp \
    spriteframe.cpp \
    controller.cpp \
    canvaslabel.cpp

HEADERS  += editorwindow.h \
    toolswidget.h \
    previewwidget.h \
    framemanagerwidget.h \
    layermanagerwidget.h \
    layerlist.h \
    framelist.h \
    sprite.h \
    spriteframe.h \
    controller.h \
    canvaslabel.h

FORMS    += editorwindow.ui \
    toolswidget.ui \
    previewwidget.ui \
    framemanagerwidget.ui \
    layermanagerwidget.ui
