cmake_minimum_required(VERSION 3.30)
project(risc_v_emulator)

set(CMAKE_CXX_STANDARD 20)

add_executable(risc_v_emulator main.cpp
        cpu.cpp
        cpu.h)
