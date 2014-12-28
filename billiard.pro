QT       += core gui opengl
QMAKE_CXXFLAGS += -std=c++11

TARGET = billiard
TEMPLATE = app

SOURCES += \
    main.cc \
    view.cc \
    system.cc \
    glsphere.cc \
    ball.cc \
    wall.cc

RESOURCES += \
    resource.qrc

HEADERS += \
    view.hh \
    system.hh \
    glsphere.hh \
    ball.hh \
    wall.hh
