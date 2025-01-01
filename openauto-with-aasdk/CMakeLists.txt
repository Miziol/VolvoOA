cmake_minimum_required(VERSION 3.5.1)
project(myopenauto)

set(CMAKE_CXX_STANDARD 14)

set(AASDK_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/aasdk/include")
set(AASDK_LIBRARIES "${CMAKE_SOURCE_DIR}/aasdk/lib/libaasdk.so")
set(AASDK_PROTO_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/aasdk")
set(AASDK_PROTO_LIBRARIES "${CMAKE_SOURCE_DIR}/aasdk/lib/libaasdk_proto.so")

add_subdirectory(aasdk)
add_subdirectory(openauto)

add_dependencies(autoapp aasdk)
add_dependencies(btservice aasdk)
