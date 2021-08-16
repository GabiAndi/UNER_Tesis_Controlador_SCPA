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
        main.cpp
