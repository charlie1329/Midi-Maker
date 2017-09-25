cmake_minimum_required(VERSION 3.8)
project(MidiMaker)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/midi.cpp src/midi.h test/catch.hpp test/testMidi.cpp)
add_executable(MidiMaker ${SOURCE_FILES})