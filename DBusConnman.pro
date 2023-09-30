QT += core widgets dbus

TEMPLATE = app

CONFIG += console c++17

SOURCES += \
        ConnmanData.cpp \
        ConnmanTechnology.cpp \
        main.cpp

HEADERS += \
    ConnmanData.hpp \
    ConnmanGlobal.hpp \
    ConnmanTechnology.hpp
