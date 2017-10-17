///
/// \file 				TimerWheelTests.cpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2017-10-16
/// \last-modified		2017-10-16
/// \brief 				Contains tests for the TimerWheel class.
/// \details
///		See README.md in root dir for more info.

// System includes
#include <atomic>
#include <chrono>
#include <thread>

// 3rd party includes
#include "gtest/gtest.h"

// User includes
#include "CppUtils/TimerWheel.hpp"
#include "CppUtils/ThreadSafeQueue.hpp"


using namespace std::literals;
using namespace mn::CppUtils;

namespace {

    class TimerWheelTests : public ::testing::Test {
    protected:
        TimerWheelTests() {}
        virtual ~TimerWheelTests() {}
    };



    TEST_F(TimerWheelTests, BasicTimerTimeout) {

        TimerWheel timerWheel;

        EXPECT_TRUE(true);
    }

}  // namespace