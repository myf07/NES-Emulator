cmake_minimum_required(VERSION 3.25)
project(P7)

set(CMAKE_CXX_STANDARD 23)

add_executable(P7 main.cpp Bus.cpp Bus.h MOS6502.cpp MOS6502.h Loader.cpp Mapper.cpp Mapper_0.cpp PPU.cpp)
