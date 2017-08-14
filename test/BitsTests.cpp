///
/// \file 				BitsTests.cpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2017-07-26
/// \last-modified		2017-08-10
/// \brief 				Contains tests for the Bits class.
/// \details
///		See README.md in root dir for more info.

// System includes
// nothing

// 3rd party includes
#include "gtest/gtest.h"

// User includes
#include "CppUtils/Bits.hpp"

using namespace mn::CppUtils;

namespace {

    class BitsTests : public ::testing::Test {
    protected:

        BitsTests() {}
        virtual ~BitsTests() {}

    };

    TEST_F(BitsTests, BitMaskTests) {
        EXPECT_EQ(0x00000000, Bits::BitMask<uint32_t>(0));
        EXPECT_EQ(0x00000001, Bits::BitMask<uint32_t>(1));
        EXPECT_EQ(0x00000003, Bits::BitMask<uint32_t>(2));
        EXPECT_EQ(0x00000007, Bits::BitMask<uint32_t>(3));
        EXPECT_EQ(0x0000000F, Bits::BitMask<uint32_t>(4));
        EXPECT_EQ(0x0000001F, Bits::BitMask<uint32_t>(5));
        EXPECT_EQ(0x000000FF, Bits::BitMask<uint32_t>(8));

        EXPECT_EQ(0x7FFFFFFF, Bits::BitMask<uint32_t>(31));
        EXPECT_EQ(0xFFFFFFFF, Bits::BitMask<uint32_t>(32));
    }

    TEST_F(BitsTests, GetBitsAllOnes) {
        EXPECT_EQ(0x00000001, Bits::GetBits(0xFFFFFFFF, 0, 1));
        EXPECT_EQ(0x00000003, Bits::GetBits(0xFFFFFFFF, 0, 2));
        EXPECT_EQ(0x00000007, Bits::GetBits(0xFFFFFFFF, 0, 3));
        EXPECT_EQ(0x0000000F, Bits::GetBits(0xFFFFFFFF, 0, 4));
        EXPECT_EQ(0x0000001F, Bits::GetBits(0xFFFFFFFF, 0, 5));

        EXPECT_EQ(0x7FFFFFFF, Bits::GetBits(0xFFFFFFFF, 0, 31));
        EXPECT_EQ(0xFFFFFFFF, Bits::GetBits(0xFFFFFFFF, 0, 32));
    }

    TEST_F(BitsTests, GetBitsNonZeroStartPos) {
        EXPECT_EQ(0x00000001, Bits::GetBits(0xF0F0F0F0, 4, 1));
        EXPECT_EQ(0x00000003, Bits::GetBits(0xF0F0F0F0, 5, 2));
        EXPECT_EQ(0x00000387, Bits::GetBits(0xF0F0F0F0, 5, 10));
    }

    TEST_F(BitsTests, SetBitsEmptyDestTest1) {
        EXPECT_EQ(0x00000001, Bits::SetBits(0x00000000, 0x1, 0, 1));
    }

    TEST_F(BitsTests, SetBitsEmptyDestTest2) {
        EXPECT_EQ(0x00000002, Bits::SetBits(0x00000000, 0x1, 1, 1));
    }

    TEST_F(BitsTests, SetBitsEmptyDestTest3) {
        EXPECT_EQ(0x80000000, Bits::SetBits(0x00000000, 0x1, 31, 1));
    }

    TEST_F(BitsTests, SetBitsFullDestTest1) {
        EXPECT_EQ(0x92345678, Bits::SetBits(0x12345678, 0x1, 31, 1));
    }

    TEST_F(BitsTests, SetBitsManySourceBitsTest1) {
        EXPECT_EQ(0x12340078u, Bits::SetBits(0x12345678, 0x00, 8, 8));
    }
}  // namespace