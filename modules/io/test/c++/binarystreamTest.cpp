#include <cxxtest/TestDrive.h>

#include "binarystreamTest_def.h"

#include <openma/io/binarystream.h>
#include <openma/io/enums.h>

CXXTEST_SUITE(BinaryStreamTest)
{
  CXXTEST_TEST(readIeeeLittleEndian)
  {
    char data[41] = {(char)0xFB, 0x01, 0x53, 0x43, 0x41, 0x4C, 0x45, 0x22, 0x00, 0x04, 0x00, (char)0xAB, (char)0xAA, (char)0xAA, 0x3D, 0x19, 0x2A, 0x20, 0x50, 0x6F, 0x69, 0x6E, 0x74, 0x20, 0x64, 0x61, 0x74, 0x61, 0x20, 0x73, 0x63, 0x61, 0x6C, 0x65, 0x20, 0x66, 0x61, 0x63, 0x74, 0x6F, 0x72};
    DummyBuffer device(data);
    ma::io::BinaryStream bs(&device, ma::io::ByteOrder::IEEELittleEndian);
    TS_ASSERT_EQUALS(bs.readI8(), (int8_t)-5);
    TS_ASSERT_EQUALS(bs.readI8(), (int8_t)1);
    TS_ASSERT_EQUALS(bs.readString(5), "SCALE");
    TS_ASSERT_EQUALS(bs.readU16(), 34);
    TS_ASSERT_EQUALS(bs.readU8(), (uint8_t)4);
    TS_ASSERT_EQUALS(bs.readU8(), (uint8_t)0);
    TS_ASSERT_DELTA(bs.readFloat(), 0.08333f, 1e-5);
    TS_ASSERT_EQUALS(bs.readU8(), (uint8_t)25);
    TS_ASSERT_EQUALS(bs.readString(25), "* Point data scale factor");
  };
  
  CXXTEST_TEST(readIeeeBigEndian)
  {
    char data[41] = {(char)0xFB, 0x01, 0x53, 0x43, 0x41, 0x4C, 0x45, 0x00, 0x22, 0x04, 0x00, 0x3D, (char)0xAA, (char)0xAA, (char)0xAB, 0x19, 0x2A, 0x20, 0x50, 0x6F, 0x69, 0x6E, 0x74, 0x20, 0x64, 0x61, 0x74, 0x61, 0x20, 0x73, 0x63, 0x61, 0x6C, 0x65, 0x20, 0x66, 0x61, 0x63, 0x74, 0x6F, 0x72};
    DummyBuffer device(data);
    ma::io::BinaryStream bs(&device, ma::io::ByteOrder::IEEEBigEndian);
    TS_ASSERT_EQUALS(bs.readI8(), (int8_t)-5);
    TS_ASSERT_EQUALS(bs.readI8(), (int8_t)1);
    TS_ASSERT_EQUALS(bs.readString(5), "SCALE");
    TS_ASSERT_EQUALS(bs.readU16(), 34);
    TS_ASSERT_EQUALS(bs.readU8(), (uint8_t)4);
    TS_ASSERT_EQUALS(bs.readU8(), (uint8_t)0);
    TS_ASSERT_DELTA(bs.readFloat(), 0.08333f, 1e-5);
    TS_ASSERT_EQUALS(bs.readU8(), (uint8_t)25);
    TS_ASSERT_EQUALS(bs.readString(25), "* Point data scale factor");
  };
  
  CXXTEST_TEST(readVaxLittleEndian)
  {
    char data[41] = {(char)0xFB, 0x01, 0x53, 0x43, 0x41, 0x4C, 0x45, 0x22, 0x00, 0x04, 0x00, (char)0xAA, 0x3E, (char)0xAB, (char)0xAA, 0x19, 0x2A, 0x20, 0x50, 0x6F, 0x69, 0x6E, 0x74, 0x20, 0x64, 0x61, 0x74, 0x61, 0x20, 0x73, 0x63, 0x61, 0x6C, 0x65, 0x20, 0x66, 0x61, 0x63, 0x74, 0x6F, 0x72};
    DummyBuffer device(data);
    ma::io::BinaryStream bs(&device, ma::io::ByteOrder::VAXLittleEndian);
    TS_ASSERT_EQUALS(bs.readI8(), (int8_t)-5);
    TS_ASSERT_EQUALS(bs.readI8(), (int8_t)1);
    TS_ASSERT_EQUALS(bs.readString(5), "SCALE");
    TS_ASSERT_EQUALS(bs.readU16(), 34);
    TS_ASSERT_EQUALS(bs.readU8(), (uint8_t)4);
    TS_ASSERT_EQUALS(bs.readU8(), (uint8_t)0);
    TS_ASSERT_DELTA(bs.readFloat(), 0.08333f, 1e-5);
    TS_ASSERT_EQUALS(bs.readU8(), (uint8_t)25);
    TS_ASSERT_EQUALS(bs.readString(25), "* Point data scale factor");
  };
  
  CXXTEST_TEST(writeReadNative)
  {
    char data[41] = {0};
    DummyBuffer device(data);
    ma::io::BinaryStream bs(&device);
    TS_ASSERT_EQUALS(bs.writeI8(-5), 1ul);
    TS_ASSERT_EQUALS(bs.writeI8(1), 1ul);
    TS_ASSERT_EQUALS(bs.writeString("SCALE"), 5ul);
    TS_ASSERT_EQUALS(bs.writeU16(34), 2ul);
    TS_ASSERT_EQUALS(bs.writeU8(4), 1ul);
    TS_ASSERT_EQUALS(bs.writeU8(0), 1ul);
    TS_ASSERT_EQUALS(bs.writeFloat(0.08333f), 4ul);
    TS_ASSERT_EQUALS(bs.writeU8(25), 1ul);
    TS_ASSERT_EQUALS(bs.writeString("* Point data scale factor"), 25ul);
    device.setPos(0); // Reset the position of the pointer
    TS_ASSERT_EQUALS(bs.readI8(), (int8_t)-5);
    TS_ASSERT_EQUALS(bs.readI8(), (int8_t)1);
    TS_ASSERT_EQUALS(bs.readString(5), "SCALE");
    TS_ASSERT_EQUALS(bs.readU16(), 34);
    TS_ASSERT_EQUALS(bs.readU8(), (uint8_t)4);
    TS_ASSERT_EQUALS(bs.readU8(), (uint8_t)0);
    TS_ASSERT_EQUALS(bs.readFloat(), 0.08333f);
    TS_ASSERT_EQUALS(bs.readU8(), (uint8_t)25);
    TS_ASSERT_EQUALS(bs.readString(25), "* Point data scale factor");
  }
};

CXXTEST_SUITE_REGISTRATION(BinaryStreamTest)
CXXTEST_TEST_REGISTRATION(BinaryStreamTest, readIeeeLittleEndian)
CXXTEST_TEST_REGISTRATION(BinaryStreamTest, readIeeeBigEndian)
CXXTEST_TEST_REGISTRATION(BinaryStreamTest, readVaxLittleEndian)
CXXTEST_TEST_REGISTRATION(BinaryStreamTest, writeReadNative)