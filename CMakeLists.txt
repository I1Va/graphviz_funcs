cmake_minimum_required(VERSION 3.10)
project(graphviz_funcs LANGUAGES CXX)

add_subdirectory(string_funcs)

add_library(graphviz_funcs STATIC
    src/dot_log.cpp
    src/graphviz_funcs.cpp
)

target_include_directories(graphviz_funcs PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(graphviz_funcs PRIVATE cxx_std_17)