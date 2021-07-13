# Configuración de Qt
QT -= gui
QT += core network

# Configuración del proyecto
CONFIG += c++11 console
CONFIG -= app_bundle

# Configuración de la instalación
target.path = /home/pi/$${TARGET}/bin
INSTALLS += target

# Archivos de recursos
SOURCES += \
        hmimanager.cpp \
        main.cpp \
        scpamanager.cpp

HEADERS += \
        hmimanager.h \
        scpamanager.h
