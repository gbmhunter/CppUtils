///
/// \file 				StrConvTests.cpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2017-08-11
/// \last-modified		2017-10-06
/// \brief 				Contains tests for the StrConv class.
/// \details
///		See README.md in root dir for more info.

// System includes
// nothing

// 3rd party includes
#include "gtest/gtest.h"

// User includes
#include "CppUtils/StrConv.hpp"

using namespace mn::CppUtils;

namespace {

    class StrConvTests : public ::testing::Test {
    protected:

        StrConvTests() {}
        virtual ~StrConvTests() {}

    };

    TEST_F(StrConvTests, HexToStringTest1) {
        EXPECT_EQ("0x01", StrConv::ToHex(0x01, 2));
    }

    TEST_F(StrConvTests, HexToStringTest2) {
        EXPECT_EQ("0xFF", StrConv::ToHex(0xFF, 2));
    }

    TEST_F(StrConvTests, HexToStringTest3) {
        EXPECT_EQ("0xABC", StrConv::ToHex(0xABC, 3));
    }

    TEST_F(StrConvTests, ToHexIterableNoValues) {
        std::vector<uint8_t> values = {};
        EXPECT_EQ("{}", StrConv::ToHex(values));
    }

    TEST_F(StrConvTests, ToHexIterableSingleValue) {
        std::vector<uint8_t> values = { 0x01 };
        EXPECT_EQ("{ 0x01 }", StrConv::ToHex(values));
    }

    TEST_F(StrConvTests, ToHexIterableMultiValue) {
        std::vector<uint8_t> values = { 0x01, 0x02, 0xAB, 0xFF };
        EXPECT_EQ("{ 0x01, 0x02, 0xAB, 0xFF }", StrConv::ToHex(values));
    }

    TEST_F(StrConvTests, ToAsciiIterableNoValues) {
        std::vector<uint8_t> values = {};
        EXPECT_EQ("{}", StrConv::ToAscii(values));
    }

    TEST_F(StrConvTests, ToAsciiIterableSingleValue) {
        std::vector<uint8_t> values = { 'a' };
        EXPECT_EQ("{ 'a' }", StrConv::ToAscii(values));
    }

    TEST_F(StrConvTests, ToAsciiIterableMultiValue) {
        std::vector<uint8_t> values = { 'a', 'b', 'c', 'd' };
        EXPECT_EQ("{ 'a', 'b', 'c', 'd' }", StrConv::ToAscii(values));
    }


}  // namespace