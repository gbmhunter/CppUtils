///
/// \file 				VerNumParserTests.cpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2018-02-04
/// \last-modified		2018-02-04
/// \brief 				Contains tests for the VerNumParser class.
/// \details
///		See README.md in root dir for more info.

// System includes
#include <chrono>

// 3rd party includes
#include <thread>
#include "gtest/gtest.h"

// Local includes
#include "CppUtils/VerNumParser.hpp"

using namespace std::literals;
using namespace mn::CppUtils;

namespace {

    class VerNumParserTests : public ::testing::Test {
    protected:
        VerNumParserTests() {}
        virtual ~VerNumParserTests() {}
    };

    TEST_F(VerNumParserTests, Basic) {
        auto digits = VerNumParser::Parse("v1.2.3");
        EXPECT_EQ(3, digits.size());
        EXPECT_EQ(1, digits[0]);
        EXPECT_EQ(2, digits[1]);
        EXPECT_EQ(3, digits[2]);
    }

    TEST_F(VerNumParserTests, NoVPrefix) {
        auto digits = VerNumParser::Parse("1.2.3");
        EXPECT_EQ(3, digits.size());
        EXPECT_EQ(1, digits[0]);
        EXPECT_EQ(2, digits[1]);
        EXPECT_EQ(3, digits[2]);
    }

    TEST_F(VerNumParserTests, JustOneDigit) {
        auto digits = VerNumParser::Parse("8");
        EXPECT_EQ(1, digits.size());
        EXPECT_EQ(8, digits[0]);
    }

    TEST_F(VerNumParserTests, TwoNumeralDigits) {
        auto digits = VerNumParser::Parse("v16.23.30");
        EXPECT_EQ(3, digits.size());
        EXPECT_EQ(16, digits[0]);
        EXPECT_EQ(23, digits[1]);
        EXPECT_EQ(30, digits[2]);
    }

    TEST_F(VerNumParserTests, FourPartVersion) {
        auto digits = VerNumParser::Parse("v0.0.2.5");
        EXPECT_EQ(4, digits.size());
        EXPECT_EQ(0, digits[0]);
        EXPECT_EQ(0, digits[1]);
        EXPECT_EQ(2, digits[2]);
        EXPECT_EQ(5, digits[3]);
    }

    TEST_F(VerNumParserTests, LeadingZero1) {
        auto digits = VerNumParser::Parse("v01.2.3");
        EXPECT_EQ(3, digits.size());
        EXPECT_EQ(1, digits[0]);
        EXPECT_EQ(2, digits[1]);
        EXPECT_EQ(3, digits[2]);
    }

    TEST_F(VerNumParserTests, LeadingZero2) {
        auto digits = VerNumParser::Parse("v01.002.0000003");
        EXPECT_EQ(3, digits.size());
        EXPECT_EQ(1, digits[0]);
        EXPECT_EQ(2, digits[1]);
        EXPECT_EQ(3, digits[2]);
    }

    TEST_F(VerNumParserTests, ExTwoVs) {        
        EXPECT_THROW(VerNumParser::Parse("vv1.2.3"), VerNumParserEx);
    }

    TEST_F(VerNumParserTests, InvalidCharAfterNumber1) {        
        EXPECT_THROW(VerNumParser::Parse("v1a.2.3"), VerNumParserEx);
    }

    TEST_F(VerNumParserTests, InvalidCharAfterNumber2) {        
        EXPECT_THROW(VerNumParser::Parse("v1.2b.3"), VerNumParserEx);
    }

    TEST_F(VerNumParserTests, InvalidCharAfterNumber3) {        
        EXPECT_THROW(VerNumParser::Parse("v1.2.3c"), VerNumParserEx);
    }

}  // namespace