Midi Maker
==========

A full implementation of the MIDI file format, implemented in C++. Midi Maker provides classes with respective member
functions for each midi command, as well as some useful utility functions. If throughout the usage of this code
you need more information on the internals of MIDI,
 I recommend the following page: [MIDI File Format](https://www.csie.ntu.edu.tw/~r92092/ref/midi/)

Repository Structure
--------------------
* `/src` contains the source files required for MIDI file creation
* `/test` contains the tests for all functionality. Tests are written using [Catch](https://github.com/philsquared/Catch)
* `/examples` contains programs written to give example usages of the library, which, when run play the resulting midi file

How To Add Midi Maker To Your Project
--------------------------------------
To add Midi Maker, simply add the following two files from `/src` to your project: `midi.h` and `midi.cpp`

How To Use Midi Maker
---------------------


How To Test Midi Maker
----------------------
If you wish to run the [Catch](https://github.com/philsquared/Catch) unit tests, simply compile Midi Maker with alongside
the file `/test/testMidi.cpp`. This should produce an executable which, when run, carries out the unit tests.
Individual tests can also be run from this executable. Please see the [Catch](https://github.com/philsquared/Catch) documentation
for details.
