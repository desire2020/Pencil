TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    pen-interfaces.cpp \
    pen-lang.cpp \
    pen-keyword.cpp

HEADERS += \
    includes/pen-interfaces.hpp \
    includes/pen-lang.hpp \
    includes/pen-utility.hpp \
    includes/suimem.hpp
