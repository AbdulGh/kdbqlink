cmake_minimum_required(VERSION 3.30)

project(kdbqlink)

set (CMAKE_BUILD_TYPE Debug)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(kdbqlink SHARED WL.cpp main.cpp q_externs.cpp q_utils.cpp)
set_property(TARGET kdbqlink PROPERTY CXX_STANDARD 23)
# TODO dont hardcode this
target_link_libraries(kdbqlink /usr/lib/libuuid.so /usr/local/Wolfram/Mathematica/13.3/SystemFiles/Links/WSTP/DeveloperKit/Linux-x86-64/CompilerAdditions/libWSTP64i4.a)
