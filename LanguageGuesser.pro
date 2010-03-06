# -------------------------------------------------
# Project created by QtCreator 2009-11-19T11:34:08
# -------------------------------------------------
# QT -= gui
TARGET = precisiontest
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += lib/trigramlanguageguesser.cpp \
    lib/simplemultilanguageguesser.cpp \
    ui/languagedetectorapp.cpp \
    #ui/main.cpp
    tests/precisiontest.cpp
HEADERS += lib/languageguesser.h \
    lib/language.h \
    lib/multilanguageguesser.h \
    lib/trigramlanguageguesser.h \
    lib/simplemultilanguageguesser.h \
    ui/languagedetectorapp.h
