cmake_minimum_required(VERSION 3.30)

project(kdbqlink)

add_library(kdbqlink SHARED qapi.cpp q_utils.cpp)
set_property(TARGET kdbqlink PROPERTY CXX_STANDARD 23)
# TODO dont hardcode this
target_link_libraries(kdbqlink /usr/lib/libuuid.so /usr/local/Wolfram/Mathematica/13.3/SystemFiles/Links/WSTP/DeveloperKit/Linux-x86-64/CompilerAdditions/libWSTP64i4.a)
