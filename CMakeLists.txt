cmake_minimum_required(VERSION 3.25)
project(P7)

set(CMAKE_CXX_STANDARD 23)

add_executable(P7 main.cpp Bus.cpp Bus.h MOS6502.cpp MOS6502.h)
set (CMAKE_CXX_FLAGS "-I/opt/homebrew/include")
#link_directories(/opt/homebrew/lib)
set (CMAKE_EXE_LINKER_FLAGS "-L/opt/homebrew/lib")
set (CMAKE_MODULE_LINKER_FLAGS "-L/opt/homebrew/lib")
set (CMAKE_SHARED_LINKER_FLAGS "-L/opt/homebrew/lib")
set (CMAKE_STATIC_LINKER_FLAGS "-L/opt/homebrew/lib")