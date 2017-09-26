Midi Maker
==========

A full implementation of the MIDI file format, implemented in C++. Midi Maker provides classes with respective member
functions for each midi command, as well as some useful utility functions. If throughout the usage of this code
you need more information on the internals of MIDI,
 I recommend the following page: [MIDI File Format](https://www.csie.ntu.edu.tw/~r92092/ref/midi/).

Repository Structure
--------------------
* `/src` contains the source files required for MIDI file creation.
* `/test` contains the tests for all functionality. Tests are written using [Catch](https://github.com/philsquared/Catch)
* `/examples` contains programs written to give example usages of the library, which, when run, play the resulting midi file.

How To Add Midi Maker To Your Project
--------------------------------------
To add Midi Maker, simply add the following two files from `/src` to your project: `midi.h` and `midi.cpp`.

How To Use Midi Maker
---------------------
There are 3 classes of use for creating Midi files. These are:

* `MidiHeader` - used for MIDI header chunks
* `MidiTrack` - used for MIDI track chunks
* `MidiFile` - used to combine chunks into a MIDI file

### MidiHeader
Every Midi file needs a Header file. Creating a header is as simple as calling the constructor

Example Usage:

```C++
int format = 0; //MIDI format, can be 0, 1 or 2
int tracks = 1; //How many track chunks do we have
int divisions = 4; //defines default unit of delta time
MidiHeader hd(format, tracks, division);
```

### MidiTrack
A MIDI file can have 1 or more track chunks. This is where the musical information is actually stored.
To create and add to a track chunk, one just has to create an object of the MidiTrack class, and can then
sequentially call whichever commands they wish. You __must__ ensure each track ends with an end of track message however.

Example Usage:

```C++
MidiTrack trk;

//meta events
trk.setTempo(0, 50000); //120bpm
trk.programChange(0, 30); //distorted electric guitar
trk.keySignature(0, 0, 0); //C Major

//note sequence
trk.noteOn(0, 0x3C, 0x40);
trk.noteOff(20, 0x3C, 0x40);

trk.noteOn(0, 0x3C, 0x40);
trk.noteOff(20, 0x3C, 0x40);

//and so on...

//end of track
trk.endOfTrack(0);
```

### MidiFile
This class brings together all of your MIDI chunks and dumps it into a file. Since a MIDI file must have at least one
Header and Track chunk, one of each are passed into the constructor. Further track chunks can be added using `addTrack()`.

Example usage:

```C++
MidiHeader hd(1, 2, 4);

MidiTrack trk1;
//add content to track

MidiTrack trk2;
//add content to track

std::string fileName = "example.mid";

MidiFile midi(fileName, hd, trk1); //add head and first track
midi.addTrack(trk2); //add second track
midi.close(); //close the midi File once finished with it
```

How To Test Midi Maker
----------------------
If you wish to run the [Catch](https://github.com/philsquared/Catch) unit tests, simply compile Midi Maker with alongside
the file `/test/testMidi.cpp`. This should produce an executable which, when run, carries out the unit tests.
Individual tests can also be run from this executable. Please see the [Catch](https://github.com/philsquared/Catch) documentation
for details.

Running The Example Code
------------------------
This code has been developed on a Windows machine. Therefore, in examples, when the file is played using `mciSendString`,
this will not work in any non-Windows machines. If you still want to run these examples, remove the following lines to
just generate the `.mid` file and then you can play the midi file using whatever you have available on your OS:

```C++
#include <Windows.h>
#include <MMSystem.h>

//...

mciSendString("play someMidi.mid",nullptr,0,nullptr);
Sleep(5000);
```
