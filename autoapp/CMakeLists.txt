cmake_minimum_required(VERSION 3.16)

project(android_auto VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(PkgConfig REQUIRED)
find_package(Qt6 COMPONENTS Core Gui Multimedia Network Qml REQUIRED)
pkg_check_modules(LibUSB REQUIRED IMPORTED_TARGET libusb-1.0)

file(GLOB_RECURSE autoapp_source_files src/backend/*.cpp src/backend/*.h src/frontend/*.qrc)

add_executable(autoapp
    src/backend/main.cpp
    ${autoapp_source_files}
)

target_link_libraries(autoapp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Multimedia
    Qt6::Network
    Qt6::Qml
    PkgConfig::LibUSB
)
