cmake_minimum_required(VERSION 3.30)
project(risc_v_emulator)

set(CMAKE_CXX_STANDARD 17)

add_executable(risc_v_emulator main.cpp
        cpu.cpp
        cpu.h)
