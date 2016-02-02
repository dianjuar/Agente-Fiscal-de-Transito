#-------------------------------------------------
#
# Project created by QtCreator 2016-01-05T15:15:19
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = prueba1
TEMPLATE = app
CONFIG   += c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    qsfmlcanvas.cpp \
    mycanvas.cpp \
    Tools/sfml.cpp \
    RVO/rvo_manager.cpp \
    entornografico.cpp \
    Tools/math.cpp \
    Agentes/agentmanager.cpp \
    Agentes/agent.cpp \
    network/MultiPropuseSockets.cpp \
    network/connection_sma.cpp \
    network/connection_aco.cpp

HEADERS  += mainwindow.h \
    qsfmlcanvas.h \
    headers/qt_stuff.h \
    headers/rvo_stuff.h \
    headers/sfml_stuff.h \
    mycanvas.h \
    sfml.h \
    Tools/sfml.h \
    RVO/rvo_manager.h \
    entornografico.h \
    Tools/math.h \
    Agentes/agentmanager.h \
    Agentes/agent.h \
    headers/opencv_stuff.h \
    network/MultiPropuseSockets.h \
    network/connection_sma.h \
    network/connection_aco.h \
    network/puertos.h \
    network/mensajes.h

FORMS    += mainwindow.ui

#-------------------------SFML-------------------------
LIBS += -L"/opt/SFML-2.3.2/lib"
CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system
CONFIG(debug, debug|release): LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-network-d -lsfml-window-d -lsfml-system-d

INCLUDEPATH += "/opt/SFML-2.3.2/include"
DEPENDPATH += "/opt/SFML-2.3.2/include"
#-------------------------SFML-------------------------
#-------------------------RVO-------------------------
LIBS += "/usr/local/lib/libRVO.a"
#-------------------------RVO-------------------------
#-------------------------OpenCV-------------------------
LIBS += `pkg-config opencv --libs`
#-------------------------OpenCV-------------------------

