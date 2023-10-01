QT += core widgets dbus

TEMPLATE = app

CONFIG += console c++17

SOURCES += \
        ConnmanData.cpp \
        ConnmanManager.cpp \
        main.cpp

HEADERS += \
    ConnmanData.hpp \
    ConnmanGlobal.hpp \
    ConnmanManager.hpp
