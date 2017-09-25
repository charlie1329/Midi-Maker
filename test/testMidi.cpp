/*
 * File to carry out all testing for my Midi Making library found in midi.cpp and midi.h
 * Tests carried out on Windows 10, test results may therefore change on different OSes
 * due to differing endian-ness
 * Author : Charlie Street
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp" //include for test framework
#include "../src/midi.h" //header file for code to test

//things to test in midi.cpp
/*
 * test a whole file plays properly (integration tests), might have to be done manually elsewhere
 * that is where I find out if I read the spec properly
 * test every single event using getData() -- we'll see ;)
 * check addTrack() in MidiFile
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

TEST_CASE("Tests (as much as possible in unit tests) that addTrack() behaves correctly", "[midiFile addTrack()]") {
    //might be a bit of a pain to test, probably gonna have to read back in from a file to test it!
    //TODO : come back to this later
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

    //--SYSEX EVENTS--

    //--CHANNEL VOICE MESSAGES--

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






}