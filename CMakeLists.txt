cmake_minimum_required(VERSION 3.14)

project(Controlador_SCPA VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6
    COMPONENTS
        Core
        Network
        Sql
    REQUIRED
)

# Libreria de protocolo de comunicación
add_subdirectory("../HMIProtocol" "../build-HMIProtocol")

qt_add_executable(${PROJECT_NAME}
    "main.cpp"
    "scpamanager.h" "scpamanager.cpp"
    "logfile.h" "logfile.cpp"
    "tuimanager.h" "tuimanager.cpp"
    "consolelistener.h" "consolelistener.cpp"
    "hmiservermanager.h" "hmiservermanager.cpp"
    "hmiusersmanager.h" "hmiusersmanager.cpp"
    "hmiclient.h" "hmiclient.cpp"
    "hmiuser.h" "hmiuser.cpp"
    "controlmanager.h" "controlmanager.cpp"
    "frequencydriver.h" "frequencydriver.cpp"
    "pidcontroller.h" "pidcontroller.cpp"
    "metricsmanager.h" "metricsmanager.cpp"
    "mysqldatabase.h" "mysqldatabase.cpp"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Core
        Qt6::Network
        Qt6::Sql
        ncurses
        wiringPi
        HMIProtocol
)

set(INSTALL_DESTDIR "/tmp/${PROJECT_NAME}")

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "${INSTALL_DESTDIR}"
    BUNDLE DESTINATION "${INSTALL_DESTDIR}"
    LIBRARY DESTINATION "${INSTALL_DESTDIR}"
)
