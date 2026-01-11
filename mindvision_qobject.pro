QT       += core gui

TARGET = mindvision_qobject
TEMPLATE = lib
CONFIG += shared

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += MINDVISION_QOBJECT_LIBRARY

# You can add the specific path to the MindVision SDK here
# e.g., INCLUDEPATH += "C:/Program Files (x86)/MindVision/MindVision-Gige/SDK/Include"
INCLUDEPATH += Include

SOURCES += \
    MindVisionCamera.cpp \
    VideoThread.cpp

HEADERS += \
    MindVisionCamera.h \
    VideoThread.h \
    mindvision_qobject_global.h

# Link against the MindVision SDK library
# Point to the Lib directory and link against the X64 library
LIBS += -L$$PWD/Lib -lMVCAMSDK_X64