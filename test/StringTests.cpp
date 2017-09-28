///
/// \file 				StringTests.cpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2017-08-11
/// \last-modified		2017-09-28
/// \brief 				Contains tests for the String class.
/// \details
///		See README.md in root dir for more info.

// System includes
// nothing

// 3rd party includes
#include "gtest/gtest.h"

// User includes
#include "CppUtils/String.hpp"

using namespace mn::CppUtils;

namespace {

    class StringTests : public ::testing::Test {
    protected:

        StringTests() {}
        virtual ~StringTests() {}

    };

    TEST_F(StringTests, HexToStringTest1) {
        EXPECT_EQ("0x01", String::ToHex(0x01, 2));
    }

    TEST_F(StringTests, HexToStringTest2) {
        EXPECT_EQ("0xFF", String::ToHex(0xFF, 2));
    }

    TEST_F(StringTests, HexToStringTest3) {
        EXPECT_EQ("0xABC", String::ToHex(0xABC, 3));
    }

    TEST_F(StringTests, ToHexIterableNoValues) {
        std::vector<uint8_t> values = {};
        EXPECT_EQ("{}", String::ToHex(values));
    }

    TEST_F(StringTests, ToHexIterableSingleValue) {
        std::vector<uint8_t> values = { 0x01 };
        EXPECT_EQ("{ 0x01 }", String::ToHex(values));
    }

    TEST_F(StringTests, ToHexIterableMultiValue) {
        std::vector<uint8_t> values = { 0x01, 0x02, 0xAB, 0xFF };
        EXPECT_EQ("{ 0x01, 0x02, 0xAB, 0xFF }", String::ToHex(values));
    }

    TEST_F(StringTests, ToAsciiIterableNoValues) {
        std::vector<uint8_t> values = {};
        EXPECT_EQ("{}", String::ToAscii(values));
    }

    TEST_F(StringTests, ToAsciiIterableSingleValue) {
        std::vector<uint8_t> values = { 'a' };
        EXPECT_EQ("{ 'a' }", String::ToAscii(values));
    }

    TEST_F(StringTests, ToAsciiIterableMultiValue) {
        std::vector<uint8_t> values = { 'a', 'b', 'c', 'd' };
        EXPECT_EQ("{ 'a', 'b', 'c', 'd' }", String::ToAscii(values));
    }


}  // namespace