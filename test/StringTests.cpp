///
/// \file 				StringTests.cpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2017-08-11
/// \last-modified		2017-08-11
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
        EXPECT_EQ("0x01", String::FromHex(0x01, 2));
    }

    TEST_F(StringTests, HexToStringTest2) {
        EXPECT_EQ("0xFF", String::FromHex(0xFF, 2));
    }

    TEST_F(StringTests, HexToStringTest3) {
        EXPECT_EQ("0xABC", String::FromHex(0xABC, 3));
    }


}  // namespace