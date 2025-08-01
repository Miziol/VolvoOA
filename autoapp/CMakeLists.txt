cmake_minimum_required(VERSION 3.16)

project(android_auto VERSION 1.0 LANGUAGES CXX)

#AASDK

set(AASDK_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/src/external/openauto-with-aasdk/aasdk/include")
set(AASDK_LIBRARIES "${CMAKE_SOURCE_DIR}/src/external/openauto-with-aasdk/aasdk/lib/libaasdk.so")
set(AASDK_PROTO_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/src/external/openauto-with-aasdk/aasdk")
set(AASDK_PROTO_LIBRARIES "${CMAKE_SOURCE_DIR}/src/external/openauto-with-aasdk/aasdk/lib/libaasdk_proto.so")

add_subdirectory(${CMAKE_SOURCE_DIR}/src/external/openauto-with-aasdk/aasdk)

#AUTOAPP

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Boost REQUIRED COMPONENTS system)
find_package(PkgConfig REQUIRED)
find_package(Protobuf REQUIRED)
find_package(Qt6 COMPONENTS Core Gui Multimedia Network Qml Bluetooth REQUIRED)
pkg_check_modules(LibUSB REQUIRED IMPORTED_TARGET libusb-1.0)

file(GLOB_RECURSE openauto_autoapp_source_files
        src/external/openauto-with-aasdk/openauto/src/autoapp/*.cpp
        src/external/openauto-with-aasdk/openauto/include/f1x/openauto/Common/*.hpp
        src/external/openauto-with-aasdk/openauto/include/f1x/openauto/autoapp/*.hpp)
file(GLOB_RECURSE autoapp_source_files src/backend/*.cpp src/backend/*.h src/frontend/*.qrc)

include_directories(
        src/external/openauto-with-aasdk/openauto/include/
        ${AASDK_INCLUDE_DIRS}
        ${AASDK_PROTO_INCLUDE_DIRS}
)

add_executable(autoapp
    src/backend/main.cpp
    ${autoapp_source_files}
    ${openauto_autoapp_source_files}
)

target_link_libraries(autoapp PRIVATE
    Boost::headers
    Qt6::Bluetooth
    Qt6::Core
    Qt6::Gui
    Qt6::Multimedia
    Qt6::Network
    Qt6::Qml
    PkgConfig::LibUSB
    ${AASDK_PROTO_LIBRARIES}
    ${AASDK_LIBRARIES}
    ${Protobuf_LIBRARIES}
)

add_dependencies(autoapp aasdk)
