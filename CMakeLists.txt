cmake_minimum_required(VERSION 3.5)

project(Controlador_SCPA VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 COMPONENTS Network REQUIRED)

set(PROJECT_SOURCES
    main.cpp
    scpamanager.h scpamanager.cpp
    logfile.h logfile.cpp
    applicationstate.h applicationstate.cpp
    tuimanager.h tuimanager.cpp
    consolelistener.h consolelistener.cpp
    hmiservermanager.h hmiservermanager.cpp
    hmiprotocolmanager.h hmiprotocolmanager.cpp
)

qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Network)
