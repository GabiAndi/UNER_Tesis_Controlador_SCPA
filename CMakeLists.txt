cmake_minimum_required(VERSION 3.14)

project(Controlador_SCPA VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Network REQUIRED)

set(PROJECT_SOURCES
    main.cpp
    scpamanager.h scpamanager.cpp
    logfile.h logfile.cpp
    tuimanager.h tuimanager.cpp
    consolelistener.h consolelistener.cpp
    hmiservermanager.h hmiservermanager.cpp
    hmiprotocolmanager.h hmiprotocolmanager.cpp
    hmiusersmanager.h hmiusersmanager.cpp
    hmiclientmanager.h hmiclientmanager.cpp
)

# Libreria de protocolo de comunicacíon
add_subdirectory("../HMIProtocol" "../build-HMIProtocol")

# Librerias externas
# ncurses
add_library(ncurses SHARED IMPORTED)
set_property(TARGET ncurses PROPERTY IMPORTED_LOCATION "/lib/x86_64-linux-gnu/libncurses.so")

qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Network)

# Libreria de protocolo de comunicacíon
target_link_libraries(${PROJECT_NAME} PRIVATE HMIProtocol)

# Librerias externas
# ncurses
target_link_libraries(${PROJECT_NAME} PRIVATE ncurses)

qt_finalize_executable(${PROJECT_NAME})
