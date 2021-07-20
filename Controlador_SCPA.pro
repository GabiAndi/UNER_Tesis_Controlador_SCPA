# Configuración de Qt
QT -= gui
QT += core network

# Configuración del proyecto
CONFIG += c++11 console
CONFIG -= app_bundle

# Configuración del deploy
target.path = /home/pi/$${TARGET}/bin
INSTALLS += target

# Archivos de recursos
SOURCES += \
        hmiclient.cpp \
        hmimanager.cpp \
        hmithread.cpp \
        main.cpp \
        scpamanager.cpp \
        scpaprotocol.cpp

HEADERS += \
        hmiclient.h \
        hmimanager.h \
        hmithread.h \
        scpamanager.h \
        scpaprotocol.h
