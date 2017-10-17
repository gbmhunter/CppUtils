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

        std::atomic<bool> timerExpiryCalled(false);
        timerWheel.AddTimer(500ms, [&]() {
            timerExpiryCalled.store(true);
        });

        std::this_thread::sleep_for(1000ms);

        EXPECT_TRUE(timerExpiryCalled.load());
    }

}  // namespace