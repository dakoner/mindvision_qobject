QT       += core gui

TARGET = _mindvision_qobject_py
QMAKE_EXTENSION_SHLIB = pyd
TEMPLATE = lib
CONFIG += shared

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += Include
INCLUDEPATH += src

# Python and pybind11 include paths
PYTHON_INCLUDE_DIR = C:/Users/davidek/scoop/apps/python313/current/Include
PYBIND11_INCLUDE_DIR = C:/Users/davidek/scoop/apps/python313/current/Lib/site-packages/pybind11/include

INCLUDEPATH += $$PYTHON_INCLUDE_DIR $$PYBIND11_INCLUDE_DIR

CONFIG += c++17

# Directory where the Python module will be placed
CONFIG(debug, debug|release) {
    DESTDIR = $$OUT_PWD/debug
} else {
    DESTDIR = $$OUT_PWD/release
}

SOURCES += src/mindvision_qobject_python.cpp

# Link against the main C++ library
CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/debug
} else {
    LIBS += -L$$PWD/release
}
LIBS += -lmindvision_qobject

# Link against the MindVision SDK
LIBS += -L$$PWD/Lib -lMVCAMSDK_X64

# Link against the Python library
PYTHON_LIB_DIR = C:/Users/davidek/scoop/apps/python313/current/libs
LIBS += -L$$PYTHON_LIB_DIR -lpython313
