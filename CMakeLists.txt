cmake_minimum_required(VERSION 3.5)

project(Controlador_SCPA VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 COMPONENTS Network REQUIRED)

set(PROJECT_SOURCES
    main.cpp
    scpamanager.cpp
    scpamanager.h
    tuimanager.cpp
    tuimanager.h
    logfile.cpp
    logfile.h
    consolelistener.cpp
    consolelistener.h
    hmiclient.cpp
    hmiclient.h
    hmiprotocolmanager.cpp
    hmiprotocolmanager.h
    hmiprotocolpackage.cpp
    hmiprotocolpackage.h
    hmiservermanager.cpp
    hmiservermanager.h
)

qt_add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Network)
