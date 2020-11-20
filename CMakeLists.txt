cmake_minimum_required(VERSION 3.17)
project(NTT C)

set(CMAKE_C_STANDARD 99)

add_executable(NTT params.h NTT.h NTT.c params.c multiplication.h multiplication.c main.c pbPlots/pbPlots.c pbPlots/supportLib.c)