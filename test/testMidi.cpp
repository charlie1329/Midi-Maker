/*
 * File to carry out all testing for my Midi Making library found in midi.cpp and midi.h
 * Tests carried out on Windows 10, test results may therefore change on different OSes
 * due to differing endian-ness
 * Author : Charlie Street
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp" //include for test framework
#include "../src/midi.h" //header file for code to test
#include <fstream>
#include <iostream>

//things to test in midi.cpp
/*
 * test a whole file plays properly (integration tests), might have to be done manually elsewhere
 * that is where I find out if I read the spec properly
 */

TEST_CASE("Tests the functionality of converting uints to a variable length quantity", "[VLQ]") {

    std::vector<byte> test1 = uintToVLQ(0); //start off simple - 0
    CHECK(test1.size() == 1);
    CHECK(test1.at(0) == 0);

    std::vector<byte> test2 = uintToVLQ(127); //max out 1 byte
    CHECK(test2.size() == 1);
    CHECK(test2.at(0) == 127);

    std::vector<byte> test3 = uintToVLQ(259); //move into a second byte
    CHECK(test3.size() == 2);
    CHECK(test3.at(0) == 130);
    CHECK(test3.at(1) == 3);

    std::vector<byte> test4 = uintToVLQ(515089); //testing out 3 bytes worth of stuff
    CHECK(test4.size() == 3);
    CHECK(test4.at(0) == 159);
    CHECK(test4.at(1) == 184);
    CHECK(test4.at(2) == 17);

    std::vector<byte> test5 = uintToVLQ(268435457); //try to go above 4 bytes
    CHECK(test5.size() == 5);
    CHECK(test5.at(0) == 129);
    CHECK(test5.at(1) == 128);
    CHECK(test5.at(2) == 128);
    CHECK(test5.at(3) == 128);
    CHECK(test5.at(4) == 1);

}

//note that at this point in time (25/09/17) I decided the null terminator IS suitable
TEST_CASE("Tests fromString behaves as intended", "[fromString]") {

    std::vector<byte> test1 = fromString("");
    CHECK(test1.at(0) == 0);
    CHECK(test1.size() == 1); //check empty string first (just null terminator)

    std::vector<byte> test2 = fromString("MThd"); //check standard string
    CHECK(test2.at(0) == 'M');
    CHECK(test2.at(1) == 'T');
    CHECK(test2.at(2) == 'h');
    CHECK(test2.at(3) == 'd');
    CHECK(test2.at(4) == 0);
    CHECK(test2.size() == 5);

    std::vector<byte> test3 = fromString("MTrk3"); //check string with numerical character
    CHECK(test3.at(0) == 'M');
    CHECK(test3.at(1) == 'T');
    CHECK(test3.at(2) == 'r');
    CHECK(test3.at(3) == 'k');
    CHECK(test3.at(4) == '3');
    CHECK(test3.at(5) == 0);
    CHECK(test3.size() == 6);
}

TEST_CASE("Tests getNthByte16 behaves correctly", "[getNthByte16]") {

    uint16_t test1 = 5; //only something in LSB
    CHECK(getNthByte16(test1,0) == 5);
    CHECK(getNthByte16(test1,1) == 0);

    uint16_t test2 = 0; //nothing in either byte
    CHECK(getNthByte16(test2,0) == 0);
    CHECK(getNthByte16(test2,1) == 0);

    uint16_t test3 = 4096; //only something in MSB
    CHECK(getNthByte16(test3,0) == 0);
    CHECK(getNthByte16(test3,1) == 16);

    uint16_t test4 = 2051; //something in both bytes
    CHECK(getNthByte16(test4,0) == 3);
    CHECK(getNthByte16(test4,1) == 8);

    //checking range of inputs allowed
    CHECK_THROWS(getNthByte16(test1,-1));
    CHECK_THROWS(getNthByte16(test1,-100));
    CHECK_THROWS(getNthByte16(test1,2));
    CHECK_THROWS(getNthByte16(test1,100));
}

TEST_CASE("Tests getNthByte32 behaves correctly", "[getNthByte32]") {

    uint32_t test1 = 0; //nothing at all
    CHECK(getNthByte32(test1,0) == 0);
    CHECK(getNthByte32(test1,1) == 0);
    CHECK(getNthByte32(test1,2) == 0);
    CHECK(getNthByte32(test1,3) == 0);

    uint32_t test2 = 17; // only in LSB
    CHECK(getNthByte32(test2,0) == 17);
    CHECK(getNthByte32(test2,1) == 0);
    CHECK(getNthByte32(test2,2) == 0);
    CHECK(getNthByte32(test2,3) == 0);

    uint32_t test3 = 6144; // only in byte 1
    CHECK(getNthByte32(test3,0) == 0);
    CHECK(getNthByte32(test3,1) == 24);
    CHECK(getNthByte32(test3,2) == 0);
    CHECK(getNthByte32(test3,3) == 0);

    uint32_t test4 = 2051; //in bytes 0 and 1
    CHECK(getNthByte32(test4,0) == 3);
    CHECK(getNthByte32(test4,1) == 8);
    CHECK(getNthByte32(test4,2) == 0);
    CHECK(getNthByte32(test4,3) == 0);

    uint32_t test5 = 65536; //in byte 2
    CHECK(getNthByte32(test5,0) == 0);
    CHECK(getNthByte32(test5,1) == 0);
    CHECK(getNthByte32(test5,2) == 1);
    CHECK(getNthByte32(test5,3) == 0);

    uint32_t test6 = 65539; //in bytes 0 and 2
    CHECK(getNthByte32(test6,0) == 3);
    CHECK(getNthByte32(test6,1) == 0);
    CHECK(getNthByte32(test6,2) == 1);
    CHECK(getNthByte32(test6,3) == 0);

    uint32_t test7 = 66051; //in bytes 0, 1 and 2
    CHECK(getNthByte32(test7,0) == 3);
    CHECK(getNthByte32(test7,1) == 2);
    CHECK(getNthByte32(test7,2) == 1);
    CHECK(getNthByte32(test7,3) == 0);

    uint32_t test8 = 134217728; //in byte 3
    CHECK(getNthByte32(test8,0) == 0);
    CHECK(getNthByte32(test8,1) == 0);
    CHECK(getNthByte32(test8,2) == 0);
    CHECK(getNthByte32(test8,3) == 8);

    uint32_t test9 = 134217737; //in bytes 0 and 3
    CHECK(getNthByte32(test9,0) == 9);
    CHECK(getNthByte32(test9,1) == 0);
    CHECK(getNthByte32(test9,2) == 0);
    CHECK(getNthByte32(test9,3) == 8);

    uint32_t test10 = 134223872; //in bytes 1 and 3
    CHECK(getNthByte32(test10,0) == 0);
    CHECK(getNthByte32(test10,1) == 24);
    CHECK(getNthByte32(test10,2) == 0);
    CHECK(getNthByte32(test10,3) == 8);

    uint32_t test11 = 134223879; //in bytes 0, 1 and 3
    CHECK(getNthByte32(test11,0) == 7);
    CHECK(getNthByte32(test11,1) == 24);
    CHECK(getNthByte32(test11,2) == 0);
    CHECK(getNthByte32(test11,3) == 8);

    uint32_t test12 = 538968064; //bytes 2 and 3
    CHECK(getNthByte32(test12,0) == 0);
    CHECK(getNthByte32(test12,1) == 0);
    CHECK(getNthByte32(test12,2) == 32);
    CHECK(getNthByte32(test12,3) == 32);

    uint32_t test13 = 2101248; //bytes 1 and 2
    CHECK(getNthByte32(test13,0) == 0);
    CHECK(getNthByte32(test13,1) == 16);
    CHECK(getNthByte32(test13,2) == 32);
    CHECK(getNthByte32(test13,3) == 0);

    uint32_t test14 = 538968069; //bytes 0, 2 and 3
    CHECK(getNthByte32(test14,0) == 5);
    CHECK(getNthByte32(test14,1) == 0);
    CHECK(getNthByte32(test14,2) == 32);
    CHECK(getNthByte32(test14,3) == 32);

    uint32_t test15 = 538972160; //bytes 1, 2 and 3
    CHECK(getNthByte32(test15,0) == 0);
    CHECK(getNthByte32(test15,1) == 16);
    CHECK(getNthByte32(test15,2) == 32);
    CHECK(getNthByte32(test15,3) == 32);

    uint32_t test16 = 538972167; //bytes 0, 1, 2, 3
    CHECK(getNthByte32(test16,0) == 7);
    CHECK(getNthByte32(test16,1) == 16);
    CHECK(getNthByte32(test16,2) == 32);
    CHECK(getNthByte32(test16,3) == 32);

    //checking range of inputs allowed
    CHECK_THROWS(getNthByte32(test1,-1));
    CHECK_THROWS(getNthByte32(test1,-100));
    CHECK_THROWS(getNthByte32(test1,4));
    CHECK_THROWS(getNthByte32(test1,100));
}

TEST_CASE("Tests (as much as possible) that writeChunk() etc. behaves correctly", "[midiFile writeChunk()]") {

    //create the test file
    std::string file = "testFile.mid";
    MidiHeader hd(0,1,45);
    MidiTrack trk;
    trk.noteOn(0,0x3C,0x40);
    trk.noteOff(100,0x3C,0x40);
    trk.endOfTrack(0);
    MidiFile midFile(file,hd,trk);
    midFile.closeFile();

    //now re-open it and check it!
    std::ifstream reOpen(file);

    std::vector<byte> expectedFile = {'M','T','h','d',0,0,0,6,0,0,0,1,0,45,
                                       'M','T','r','k',0,0,0,12,0,0x90,0x3C,0x40,
                                        100,0x80,0x3C,0x40,0,0xFF,0x2F,0x00};

    //reading file in as a whole to check each byte
    //credit to: http://www.cplusplus.com/forum/beginner/21548/
    //get size of file
    reOpen.seekg(0,std::ios::end);
    size_t size = (size_t)reOpen.tellg();
    reOpen.seekg(0,std::ios::beg);

    REQUIRE(size == expectedFile.size());

    char oData[size];
    reOpen.read(oData,size);//read in size bytes

    //go through each byte and compare
    for(int i = 0; i < size; i++) {
        CHECK((byte)oData[i] == expectedFile.at(i)); //do a cast to make sure equality works fine
    }

    reOpen.close();
}

TEST_CASE("Tests header file set up follows the MIDI specification", "[Header]") {

    /* if this constructor sets up everything correctly,
     * MidiChunk is fine and implicitly the
     * MidiTrack class is fine
     */

    MidiHeader hd(1,3,45); //initialise an object of class MidiHeader to use throughout this test

    std::vector<byte> type = hd.getType(); //checking head stored properly
    CHECK(type.size() == 4);
    CHECK(type.at(0) == 'M');
    CHECK(type.at(1) == 'T');
    CHECK(type.at(2) == 'h');
    CHECK(type.at(3) == 'd');

    uint32_t length = hd.getLength();
    CHECK(length == 6); //should always be 6!

    std::vector<byte> data = hd.getData();
    CHECK(data.size() == 6);
    CHECK(data.at(0) == 0);
    CHECK(data.at(1) == 1);
    CHECK(data.at(2) == 0);
    CHECK(data.at(3) == 3);
    CHECK(data.at(4) == 0);
    CHECK(data.at(5) == 45);
}


TEST_CASE("Checks general behaviour of each Midi 'command'", "[Midi commands]") {
    //--META EVENTS--
    MidiTrack testMeta1;
    testMeta1.sequenceNum(7,35);
    std::vector<byte> sequenceNum = testMeta1.getData();
    CHECK(sequenceNum.size() == 6);
    CHECK(sequenceNum.at(0) == 7);
    CHECK(sequenceNum.at(1) == 255);
    CHECK(sequenceNum.at(2) == 0);
    CHECK(sequenceNum.at(3) == 2);
    CHECK(sequenceNum.at(4) == 0);
    CHECK(sequenceNum.at(5) == 35);

    MidiTrack testMeta2;
    testMeta2.textEvent(7,"hi");
    std::vector<byte> textEvent = testMeta2.getData();
    CHECK(textEvent.size() == 7);
    CHECK(textEvent.at(0) == 7);
    CHECK(textEvent.at(1) == 255);
    CHECK(textEvent.at(2) == 1);
    CHECK(textEvent.at(3) == 3);
    CHECK(textEvent.at(4) == 'h');
    CHECK(textEvent.at(5) == 'i');
    CHECK(textEvent.at(6) == 0);

    MidiTrack testMeta3;
    testMeta3.copyrightNotice(7,"(C)");
    std::vector<byte> copyright = testMeta3.getData();
    CHECK(copyright.size() == 8);
    CHECK(copyright.at(0) == 7);
    CHECK(copyright.at(1) == 255);
    CHECK(copyright.at(2) == 2);
    CHECK(copyright.at(3) == 4);
    CHECK(copyright.at(4) == '(');
    CHECK(copyright.at(5) == 'C');
    CHECK(copyright.at(6) == ')');
    CHECK(copyright.at(7) == 0);

    MidiTrack testMeta4;
    testMeta4.trackName(7,"trk");
    std::vector<byte> trackName = testMeta4.getData();
    CHECK(trackName.size() == 8);
    CHECK(trackName.at(0) == 7);
    CHECK(trackName.at(1) == 255);
    CHECK(trackName.at(2) == 3);
    CHECK(trackName.at(3) == 4);
    CHECK(trackName.at(4) == 't');
    CHECK(trackName.at(5) == 'r');
    CHECK(trackName.at(6) == 'k');
    CHECK(trackName.at(7) == 0);

    MidiTrack testMeta5;
    testMeta5.instrumentName(7,"gtr");
    std::vector<byte> instrName = testMeta5.getData();
    CHECK(instrName.size() == 8);
    CHECK(instrName.at(0) == 7);
    CHECK(instrName.at(1) == 255);
    CHECK(instrName.at(2) == 4);
    CHECK(instrName.at(3) == 4);
    CHECK(instrName.at(4) == 'g');
    CHECK(instrName.at(5) == 't');
    CHECK(instrName.at(6) == 'r');
    CHECK(instrName.at(7) == 0);

    MidiTrack testMeta6;
    testMeta6.lyric(7,"hey");
    std::vector<byte> lyric = testMeta6.getData();
    CHECK(lyric.size() == 8);
    CHECK(lyric.at(0) == 7);
    CHECK(lyric.at(1) == 255);
    CHECK(lyric.at(2) == 5);
    CHECK(lyric.at(3) == 4);
    CHECK(lyric.at(4) == 'h');
    CHECK(lyric.at(5) == 'e');
    CHECK(lyric.at(6) == 'y');
    CHECK(lyric.at(7) == 0);

    MidiTrack testMeta7;
    testMeta7.marker(7,"vs1");
    std::vector<byte> marker = testMeta7.getData();
    CHECK(marker.size() == 8);
    CHECK(marker.at(0) == 7);
    CHECK(marker.at(1) == 255);
    CHECK(marker.at(2) == 6);
    CHECK(marker.at(3) == 4);
    CHECK(marker.at(4) == 'v');
    CHECK(marker.at(5) == 's');
    CHECK(marker.at(6) == '1');
    CHECK(marker.at(7) == 0);

    MidiTrack testMeta8;
    testMeta8.cuePoint(7,"esr");
    std::vector<byte> cue = testMeta8.getData();
    CHECK(cue.size() == 8);
    CHECK(cue.at(0) == 7);
    CHECK(cue.at(1) == 255);
    CHECK(cue.at(2) == 7);
    CHECK(cue.at(3) == 4);
    CHECK(cue.at(4) == 'e');
    CHECK(cue.at(5) == 's');
    CHECK(cue.at(6) == 'r');
    CHECK(cue.at(7) == 0);

    MidiTrack testMeta9;
    testMeta9.midiChannelPrefix(7,0x00);
    std::vector<byte> midiPrefix = testMeta9.getData();
    CHECK(midiPrefix.size() == 5);
    CHECK(midiPrefix.at(0) == 7);
    CHECK(midiPrefix.at(1) == 255);
    CHECK(midiPrefix.at(2) == 32);
    CHECK(midiPrefix.at(3) == 1);
    CHECK(midiPrefix.at(4) == 0);

    MidiTrack testMeta10;
    testMeta10.endOfTrack(7);
    std::vector<byte> eot = testMeta10.getData();
    CHECK(eot.size() == 4);
    CHECK(eot.at(0) == 7);
    CHECK(eot.at(1) == 255);
    CHECK(eot.at(2) == 47);
    CHECK(eot.at(3) == 0);

    MidiTrack testMeta11;
    testMeta11.setTempo(7,50000);
    std::vector<byte> tempo = testMeta11.getData();
    CHECK(tempo.size() == 7);
    CHECK(tempo.at(0) == 7);
    CHECK(tempo.at(1) == 255);
    CHECK(tempo.at(2) == 81);
    CHECK(tempo.at(3) == 03);
    CHECK(tempo.at(4) == 0);
    CHECK(tempo.at(5) == 195);
    CHECK(tempo.at(6) == 80);

    MidiTrack testMeta12;
    testMeta12.smtpeOffset(7,64,100);
    std::vector<byte> smtpe = testMeta12.getData();
    CHECK(smtpe.size() == 9);
    CHECK(smtpe.at(0) == 7);
    CHECK(smtpe.at(1) == 255);
    CHECK(smtpe.at(2) == 84);
    CHECK(smtpe.at(3) == 5);
    CHECK(smtpe.at(4) == 0);
    CHECK(smtpe.at(5) == 0);
    CHECK(smtpe.at(6) == 0);
    CHECK(smtpe.at(7) == 64);
    CHECK(smtpe.at(8) == 100);

    MidiTrack testMeta13;
    testMeta13.timeSignature(7,6,3,24,8);
    std::vector<byte> timeSig = testMeta13.getData();
    CHECK(timeSig.size() == 8);
    CHECK(timeSig.at(0) == 7);
    CHECK(timeSig.at(1) == 255);
    CHECK(timeSig.at(2) == 88);
    CHECK(timeSig.at(3) == 4);
    CHECK(timeSig.at(4) == 6);
    CHECK(timeSig.at(5) == 3);
    CHECK(timeSig.at(6) == 24);
    CHECK(timeSig.at(7) == 8);

    MidiTrack testMeta14;
    testMeta14.keySignature(7,1,0);//G Major
    std::vector<byte> keySig = testMeta14.getData();
    CHECK(keySig.size() == 6);
    CHECK(keySig.at(0) == 7);
    CHECK(keySig.at(1) == 255);
    CHECK(keySig.at(2) == 89);
    CHECK(keySig.at(3) == 2);
    CHECK(keySig.at(4) == 1);
    CHECK(keySig.at(5) == 0);

    MidiTrack testMeta15;
    testMeta15.sequencerSpecificMetaEvent(7,{0x10},{0x12,0x11});
    std::vector<byte> ssme = testMeta15.getData();
    CHECK(ssme.size() == 7);
    CHECK(ssme.at(0) == 7);
    CHECK(ssme.at(1) == 255);
    CHECK(ssme.at(2) == 127);
    CHECK(ssme.at(3) == 3);
    CHECK(ssme.at(4) == 16);
    CHECK(ssme.at(5) == 18);
    CHECK(ssme.at(6) == 17);

    //--SYSEX EVENTS--
    MidiTrack testSysex1;
    testSysex1.f0Sysex(7, {0x10, 0x15, 0xA0});
    std::vector<byte> f0Sysex = testSysex1.getData();
    CHECK(f0Sysex.size() == 6);
    CHECK(f0Sysex.at(0) == 7);
    CHECK(f0Sysex.at(1) == 240);
    CHECK(f0Sysex.at(2) == 3);
    CHECK(f0Sysex.at(3) == 16);
    CHECK(f0Sysex.at(4) == 21);
    CHECK(f0Sysex.at(5) == 160);

    MidiTrack testSysex2;
    testSysex2.f7Sysex(7, {0x10, 0x15, 0xA0});
    std::vector<byte> f7Sysex = testSysex2.getData();
    CHECK(f7Sysex.size() == 6);
    CHECK(f7Sysex.at(0) == 7);
    CHECK(f7Sysex.at(1) == 247);
    CHECK(f7Sysex.at(2) == 3);
    CHECK(f7Sysex.at(3) == 16);
    CHECK(f7Sysex.at(4) == 21);
    CHECK(f7Sysex.at(5) == 160);


    //--CHANNEL VOICE MESSAGES--
    MidiTrack testVoice1;
    testVoice1.noteOff(7,0x3C,0x7F);
    std::vector<byte> noteOff = testVoice1.getData();
    CHECK(noteOff.size() == 4);
    CHECK(noteOff.at(0) == 7);
    CHECK(noteOff.at(1) == 128);
    CHECK(noteOff.at(2) == 60);
    CHECK(noteOff.at(3) == 127);

    MidiTrack testVoice2;
    testVoice2.noteOn(7,0x3C,0x7F);
    std::vector<byte> noteOn = testVoice2.getData();
    CHECK(noteOn.size() == 4);
    CHECK(noteOn.at(0) == 7);
    CHECK(noteOn.at(1) == 144);
    CHECK(noteOn.at(2) == 60);
    CHECK(noteOn.at(3) == 127);

    MidiTrack testVoice3;
    testVoice3.polyphonicKeyPressure(7, 0x3C, 0x00);
    std::vector<byte> polyKeyPressure = testVoice3.getData();
    CHECK(polyKeyPressure.size() == 4);
    CHECK(polyKeyPressure.at(0) == 7);
    CHECK(polyKeyPressure.at(1) == 160);
    CHECK(polyKeyPressure.at(2) == 60);
    CHECK(polyKeyPressure.at(3) == 0);

    MidiTrack testVoice4;
    testVoice4.controllerChange(7, 0x77, 0x40);
    std::vector<byte> controlChange = testVoice4.getData();
    CHECK(controlChange.size() == 4);
    CHECK(controlChange.at(0) == 7);
    CHECK(controlChange.at(1) == 176);
    CHECK(controlChange.at(2) == 119);
    CHECK(controlChange.at(3) == 64);

    MidiTrack testVoice5;
    testVoice5.programChange(7,0x40);
    std::vector<byte> progChange = testVoice5.getData();
    CHECK(progChange.size() == 3);
    CHECK(progChange.at(0) == 7);
    CHECK(progChange.at(1) == 192);
    CHECK(progChange.at(2) == 64);

    MidiTrack testVoice6;
    testVoice6.channelKeyPressure(7,0x40);
    std::vector<byte> channelPressure = testVoice6.getData();
    CHECK(channelPressure.size() == 3);
    CHECK(channelPressure.at(0) == 7);
    CHECK(channelPressure.at(1) == 208);
    CHECK(channelPressure.at(2) == 64);

    MidiTrack testVoice7;
    testVoice7.pitchBend(7, 0x40, 0x7F);
    std::vector<byte> pitchBend = testVoice7.getData();
    CHECK(pitchBend.size() == 4);
    CHECK(pitchBend.at(0) == 7);
    CHECK(pitchBend.at(1) == 224);
    CHECK(pitchBend.at(2) == 64);
    CHECK(pitchBend.at(3) == 127);

    //--CHANNEL MODE MESSAGES--
    MidiTrack testMode1;
    testMode1.allSoundOff(7);
    std::vector<byte> soundOff = testMode1.getData();
    CHECK(soundOff.size() == 4);
    CHECK(soundOff.at(0) == 7);
    CHECK(soundOff.at(1) == 176);
    CHECK(soundOff.at(2) == 120);
    CHECK(soundOff.at(3) == 0);

    MidiTrack testMode2;
    testMode2.resetAllControllers(7);
    std::vector<byte> resetCon = testMode2.getData();
    CHECK(resetCon.size() == 4);
    CHECK(resetCon.at(0) == 7);
    CHECK(resetCon.at(1) == 176);
    CHECK(resetCon.at(2) == 121);
    CHECK(resetCon.at(3) == 0);

    MidiTrack testMode3;
    testMode3.localControl(7,0x7F);
    std::vector<byte> localCon = testMode3.getData();
    CHECK(localCon.size() == 4);
    CHECK(localCon.at(0) == 7);
    CHECK(localCon.at(1) == 176);
    CHECK(localCon.at(2) == 122);
    CHECK(localCon.at(3) == 127);

    MidiTrack testMode4;
    testMode4.allNotesOff(7);
    std::vector<byte> notesOff = testMode4.getData();
    CHECK(notesOff.size() == 4);
    CHECK(notesOff.at(0) == 7);
    CHECK(notesOff.at(1) == 176);
    CHECK(notesOff.at(2) == 123);
    CHECK(notesOff.at(3) == 0);

    MidiTrack testMode5;
    testMode5.omniModeOff(7);
    std::vector<byte> omniOff = testMode5.getData();
    CHECK(omniOff.size() == 4);
    CHECK(omniOff.at(0) == 7);
    CHECK(omniOff.at(1) == 176);
    CHECK(omniOff.at(2) == 124);
    CHECK(omniOff.at(3) == 0);

    MidiTrack testMode6;
    testMode6.omniModeOn(7);
    std::vector<byte> omniOn = testMode6.getData();
    CHECK(omniOn.size() == 4);
    CHECK(omniOn.at(0) == 7);
    CHECK(omniOn.at(1) == 176);
    CHECK(omniOn.at(2) == 125);
    CHECK(omniOn.at(3) == 0);

    MidiTrack testMode7;
    testMode7.monoModeOn(7, 9);
    std::vector<byte> monoOn = testMode7.getData();
    CHECK(monoOn.size() == 4);
    CHECK(monoOn.at(0) == 7);
    CHECK(monoOn.at(1) == 176);
    CHECK(monoOn.at(2) == 126);
    CHECK(monoOn.at(3) == 9);

    MidiTrack testMode8;
    testMode8.polyModeOn(7);
    std::vector<byte> polyOn = testMode8.getData();
    CHECK(polyOn.size() == 4);
    CHECK(polyOn.at(0) == 7);
    CHECK(polyOn.at(1) == 176);
    CHECK(polyOn.at(2) == 127);
    CHECK(polyOn.at(3) == 0);

    //--MULTIPLE MESSAGE TEST--

    MidiTrack testMulti;
    testMulti.noteOn(0,0x3C,0x40);
    testMulti.noteOff(100,0x3C,0x40);
    std::vector<byte> multi = testMulti.getData();
    CHECK(multi.size() == 8);
    CHECK(multi.at(0) == 0);
    CHECK(multi.at(1) == 144);
    CHECK(multi.at(2) == 60);
    CHECK(multi.at(3) == 64);
    CHECK(multi.at(4) == 100);
    CHECK(multi.at(5) == 128);
    CHECK(multi.at(6) == 60);
    CHECK(multi.at(7) == 64);

}