QT       += core gui

TARGET = _mindvision_qobject_py
TEMPLATE = lib
CONFIG += shared

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += Include
INCLUDEPATH += src

# Python and pybind11 include paths
PYTHON_INCLUDE_DIR = /usr/include/python3.11
PYBIND11_INCLUDE_DIR = /usr/include/pybind11

INCLUDEPATH += $$PYTHON_INCLUDE_DIR $$PYBIND11_INCLUDE_DIR

CONFIG += c++17

# Directory where the Python module will be placed
CONFIG(debug, debug|release) {
    DESTDIR = $$OUT_PWD/debug
    OBJECTS_DIR = $$OUT_PWD/debug
    MOC_DIR = $$OUT_PWD/debug
} else {
    DESTDIR = $$OUT_PWD/release
    OBJECTS_DIR = $$OUT_PWD/release
    MOC_DIR = $$OUT_PWD/release
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
LIBS += -L$$PWD/Lib -lMVSDK

# Link against the Python library
win32: {
    PYTHON_LIB_DIR = C:/Users/davidek/scoop/apps/python313/current/libs
    LIBS += -L$$PYTHON_LIB_DIR -lpython3.11
} else: {
    LIBS += -lpython3.11
}

unix:!macx: {
    QMAKE_POST_LINK += $$QMAKE_MOVE $$shell_path($$DESTDIR/lib_mindvision_qobject_py.so) $$shell_path($$DESTDIR/_mindvision_qobject_py.so)
}
