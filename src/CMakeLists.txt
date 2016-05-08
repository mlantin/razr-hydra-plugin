cmake_minimum_required(VERSION 3.4)

project(razr-hydra-plugin)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/../bin)
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set (LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set (EXT_LIB "${CMAKE_SOURCE_DIR}/../lib")
include_directories(${PROJECT_SOURCE_DIR} ${EXT_LIB})

add_library(sixense SHARED IMPORTED)
set_property(TARGET sixense PROPERTY IMPORTED_LOCATION "/usr/local/lib/libsixense_x64.dylib")

add_library(protobuf_lib STATIC IMPORTED)
set_property(TARGET protobuf_lib PROPERTY IMPORTED_LOCATION /usr/local/lib/libprotobuf.a)

add_subdirectory(protobuf)
add_subdirectory(easywsclient)

add_executable(hydra-plugin hydra-plugin.cpp)

target_link_libraries(hydra-plugin sixense easywsclient VRCom protobuf_lib)

target_compile_features(hydra-plugin PUBLIC cxx_range_for)