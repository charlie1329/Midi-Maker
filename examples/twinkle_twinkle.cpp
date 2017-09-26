/*
 * An example usage of the MidiMaker library.
 * Will attempt to produce twinkle twinkle little star in midi
 * Author: Charlie Street
 */

#include "../src/midi.h"
#include <Windows.h>
#include <MMSystem.h>

int main(int argc, char **argv) {

    std::string fileName = "twinkle_twinkle.mid";

    uint16_t division = 4;
    unsigned int quarterNote = 20;

    MidiHeader hd(0,1,division);
    MidiTrack trk;

    //meta events
    trk.setTempo(0,50000); //120bpm
    trk.programChange(0,30); //electric guitar! (mci seems to follow general midi quite closely)
    trk.keySignature(0,0,0); //C Major

    //note sequence
    trk.noteOn(0,0x3C,0x40); //twin
    trk.noteOff(quarterNote,0x3C,0x40);

    trk.noteOn(0,0x3C,0x40); //kle
    trk.noteOff(quarterNote,0x3C,0x40);

    trk.noteOn(0,0x43,0x40); //twin
    trk.noteOff(quarterNote,0x43,0x40);

    trk.noteOn(0,0x43,0x40); //kle
    trk.noteOff(quarterNote,0x43,0x40);

    trk.noteOn(0,0x45,0x40); //lit
    trk.noteOff(quarterNote,0x45,0x40);

    trk.noteOn(0,0x45,0x40); //tle
    trk.noteOff(quarterNote,0x45,0x40);

    trk.noteOn(0,0x43,0x40); //star
    trk.noteOff(quarterNote,0x43,0x40);

    trk.noteOn(quarterNote,0x41,0x40); //how
    trk.noteOff(quarterNote,0x41,0x40);

    trk.noteOn(0,0x41,0x40); //i
    trk.noteOff(quarterNote,0x41,0x40);

    trk.noteOn(0,0x40,0x40); //won
    trk.noteOff(quarterNote,0x40,0x40);

    trk.noteOn(0,0x40,0x40); //der
    trk.noteOff(quarterNote,0x40,0x40);

    trk.noteOn(0,0x3E,0x40); //what
    trk.noteOff(quarterNote,0x3E,0x40);

    trk.noteOn(0,0x3E,0x40); //you
    trk.noteOff(quarterNote,0x3E,0x40);

    trk.noteOn(0,0x3C,0x40); //are
    trk.noteOff(quarterNote,0x3C,0x40);

    //end of track
    trk.endOfTrack(0);

    //write to file
    MidiFile output(fileName,hd,trk);
    output.closeFile();

    //play it!
    mciSendString("play twinkle_twinkle.mid",nullptr,0,nullptr);
    Sleep(5000);
    return 0;
}