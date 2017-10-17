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

    TEST_F(TimerWheelTests, SingleTimer) {
        TimerWheel timerWheel;

        std::atomic<bool> timerExpiryCalled(false);
        timerWheel.AddTimer(500ms, [&]() {
            timerExpiryCalled.store(true);
        });

        std::this_thread::sleep_for(1000ms);

        EXPECT_TRUE(timerExpiryCalled.load());
    }

    TEST_F(TimerWheelTests, TwoTimers) {
        TimerWheel timerWheel;

        std::atomic<int> counter(0);

        timerWheel.AddTimer(100ms, [&]() {
            counter.fetch_add(1);
        });
        timerWheel.AddTimer(500ms, [&]() {
            counter.fetch_add(1);
        });

        std::this_thread::sleep_for(1000ms);
        EXPECT_EQ(2, counter.load());
    }

    TEST_F(TimerWheelTests, ThreeTimers) {
        TimerWheel timerWheel;

        std::atomic<int> counter(0);

        timerWheel.AddTimer(100ms, [&]() {
            counter.fetch_add(1);
        });
        timerWheel.AddTimer(500ms, [&]() {
            counter.fetch_add(1);
        });
        timerWheel.AddTimer(50ms, [&]() {
            counter.fetch_add(1);
        });


        std::this_thread::sleep_for(1000ms);
        EXPECT_EQ(3, counter.load());
    }

    TEST_F(TimerWheelTests, TwoExpiredOneStillRunning) {
        TimerWheel timerWheel;

        std::atomic<int> counter(0);

        auto timer0 = timerWheel.AddTimer(100ms, [&]() {
            counter.fetch_add(1);
        });
        auto timer1 = timerWheel.AddTimer(2000ms, [&]() {
            counter.fetch_add(1);
        });
        auto timer2 = timerWheel.AddTimer(50ms, [&]() {
            counter.fetch_add(1);
        });

        std::cout << "timer0 = " << timer0.get() << ", timer1 = " << timer1.get() << ", timer2 = " << timer2.get() << std::endl;

        std::this_thread::sleep_for(1000ms);
        EXPECT_EQ(2, counter.load());
    }

}  // namespace