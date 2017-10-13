///
/// \file 				HeapTrackerTests.cpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2017-10-13
/// \last-modified		2017-10-13
/// \brief 				Contains tests for the HeapTracker class.
/// \details
///		See README.md in root dir for more info.

// System includes
// nothing

// 3rd party includes
#include "gtest/gtest.h"

// User includes
#include "CppUtils/HeapTracker.hpp"

// NOTE: These "HeapTracker" enabled new and delete functions will be used for all tests, but that's not such a big
// problem, because that is a test in itself!
HEAP_TRACKER_NEW;
HEAP_TRACKER_DELETE;

using namespace mn::CppUtils;

namespace {

    class HeapTrackerTests : public ::testing::Test {
    protected:

        HeapTrackerTests() {}
        virtual ~HeapTrackerTests() {}

    };

    TEST_F(HeapTrackerTests, ArrayAllocation) {
        auto heapSize1_B = HeapTracker::Instance().GetHeapSize_B();
        uint8_t* tempBuffer = new uint8_t[100];
        auto heapSize2_B = HeapTracker::Instance().GetHeapSize_B();
        EXPECT_EQ(100, (int64_t)heapSize2_B - (int64_t)heapSize1_B);
        delete[] tempBuffer;
        auto heapSize3_B = HeapTracker::Instance().GetHeapSize_B();
        EXPECT_EQ(-100, (int64_t)heapSize3_B - (int64_t)heapSize2_B);
    }

    TEST_F(HeapTrackerTests, SingleVarAndArrayAllocations) {
        auto heapSize1_B = HeapTracker::Instance().GetHeapSize_B();
        uint32_t* tempVal = new uint32_t;
        uint8_t* tempBuffer = new uint8_t[100];
        auto heapSize2_B = HeapTracker::Instance().GetHeapSize_B();
        EXPECT_EQ(104, (int64_t)heapSize2_B - (int64_t)heapSize1_B);
        delete tempVal;
        delete[] tempBuffer;
        auto heapSize3_B = HeapTracker::Instance().GetHeapSize_B();
        EXPECT_EQ(-104, (int64_t)heapSize3_B - (int64_t)heapSize2_B);
    }

}  // namespace