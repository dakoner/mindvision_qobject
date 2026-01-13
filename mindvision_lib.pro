QT       += core gui

TARGET = mindvision_qobject
TEMPLATE = lib
CONFIG += shared

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += MINDVISION_QOBJECT_LIBRARY

INCLUDEPATH += Include
INCLUDEPATH += src

PYTHON_INCLUDE_DIR = C:/Users/davidek/scoop/apps/python313/current/Include
PYBIND11_INCLUDE_DIR = C:/Users/davidek/scoop/apps/python313/current/Lib/site-packages/pybind11/include

INCLUDEPATH += $$PYTHON_INCLUDE_DIR $$PYBIND11_INCLUDE_DIR

CONFIG += c++17

SOURCES += \
    src/MindVisionCamera.cpp \
    src/VideoThread.cpp

HEADERS += \
    src/MindVisionCamera.h \
    src/VideoThread.h \
    src/mindvision_qobject_global.h

LIBS += -L$$PWD/Lib -lMVCAMSDK_X64
