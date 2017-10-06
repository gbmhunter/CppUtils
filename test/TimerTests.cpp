///
/// \file 				TimerTests.cpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2017-10-06
/// \last-modified		2017-10-06
/// \brief 				Contains tests for the Timer class.
/// \details
///		See README.md in root dir for more info.

// System includes
#include <atomic>
#include <chrono>
#include <thread>

// 3rd party includes
#include "gtest/gtest.h"

// User includes
#include "CppUtils/Timer.hpp"

using namespace std::literals;
using namespace mn::CppUtils;

namespace {

    class TimerTests : public ::testing::Test {
    protected:
        TimerTests() {}
        virtual ~TimerTests() {}
    };

    TEST_F(TimerTests, BasicTimerTimeout) {
        std::atomic<bool> callbackCalled(false);
        Timer timer(100ms, [&]{
            callbackCalled.store(true);
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        EXPECT_TRUE(callbackCalled.load());
    }

    TEST_F(TimerTests, DestroyBeforeTimeout) {
        std::atomic<bool> callbackCalled(false);

        {
            Timer timer(100ms, [&] {
                callbackCalled.store(true);
            });
        } // Timer gets destroyed here before timeout occurs!

        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        EXPECT_FALSE(callbackCalled.load());
    }

}  // namespace