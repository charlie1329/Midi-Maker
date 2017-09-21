cmake_minimum_required(VERSION 3.8)
project(MidiMaker)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES midi.cpp midi.h)
add_executable(MidiMaker ${SOURCE_FILES})