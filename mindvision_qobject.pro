QT       += core gui

TARGET = mindvision_qobject
TEMPLATE = lib
CONFIG += shared

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += MINDVISION_QOBJECT_LIBRARY

# You can add the specific path to the MindVision SDK here
# e.g., INCLUDEPATH += "C:/Program Files (x86)/MindVision/MindVision-Gige/SDK/Include"
INCLUDEPATH += Include

PYTHON_INCLUDE_DIR = C:/Users/davidek/scoop/apps/python313/current/Include
PYBIND11_INCLUDE_DIR = C:/Users/davidek/scoop/apps/python313/current/Lib/site-packages/pybind11/include

INCLUDEPATH += $$PYTHON_INCLUDE_DIR $$PYBIND11_INCLUDE_DIR

QMAKE_CXXFLAGS += -std=c++17

INCLUDEPATH += src

SOURCES += \
    src/MindVisionCamera.cpp \
    src/VideoThread.cpp

HEADERS += \
    src/MindVisionCamera.h \
    src/VideoThread.h \
    src/mindvision_qobject_global.h

# Link against the MindVision SDK library
# Point to the Lib directory and link against the X64 library
LIBS += -L$$PWD/Lib -lMVCAMSDK_X64

######################################################################
# Python Wrapper Generation (pybind11)
######################################################################

# Add Python and pybind11 include paths
PYTHON_INCLUDE_DIR = C:/Users/davidek/scoop/apps/python313/current/Include
PYBIND11_INCLUDE_DIR = C:/Users/davidek/scoop/apps/python313/current/Lib/site-packages/pybind11/include

INCLUDEPATH += $$PYTHON_INCLUDE_DIR $$PYBIND11_INCLUDE_DIR

# Set C++ standard for pybind11
QMAKE_CXXFLAGS += -std=c++17

# Python Wrapper Module
python_wrapper {
    # Set the target name for the Python module (e.g., _mymodule.pyd or _mymodule.so)
    TARGET = _mindvision_qobject_py
    TEMPLATE = lib
    CONFIG += shared

    # Directory where the Python module will be placed
    CONFIG(debug, debug|release) {
        DESTDIR = $$OUT_PWD/debug
    } else {
        DESTDIR = $$OUT_PWD/release
    }

    # Add the pybind11 wrapper source file
    SOURCES += src/mindvision_qobject_python.cpp

    # Link against the main C++ library
    CONFIG(debug, debug|release) {
        LIBS += -L$$OUT_PWD/debug
    } else {
        LIBS += -L$$OUT_PWD/release
    }
    LIBS += -lmindvision_qobject

    # Link against the MindVision SDK as well, as the wrapper uses MindVisionCamera.
    LIBS += -L$$PWD/Lib -lMVCAMSDK_X64

    # Link against the Python library
    PYTHON_LIB_DIR = C:/Users/davidek/scoop/apps/python313/current/libs
    LIBS += -L$$PYTHON_LIB_DIR -lpython313
}