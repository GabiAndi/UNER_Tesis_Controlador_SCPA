# Configuración de Qt
QT -= gui
QT += core network

# Configuración del proyecto
CONFIG += c++11 console
CONFIG -= app_bundle

# Configuración del deploy para Raspberry
target.path = /home/pi/$${TARGET}/bin
INSTALLS += target

# Librerias
LIBS +=

# Archivos de recursos
SOURCES += \
        consolelistener.cpp \
        hmiclient.cpp \
        hmiprotocolmanager.cpp \
        hmiservermanager.cpp \
        logfile.cpp \
        main.cpp \
        scpamanager.cpp \
        tuimanager.cpp

HEADERS += \
        consolelistener.h \
        hmiclient.h \
        hmiprotocolmanager.h \
        hmiservermanager.h \
        logfile.h \
        scpamanager.h \
        tuimanager.h
