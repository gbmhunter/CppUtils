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
using namespace mn::CppUtils::TimerWheel;

namespace {

    class TimerWheelTests : public ::testing::Test {
    protected:
        TimerWheelTests() {}
        virtual ~TimerWheelTests() {}
    };

    TEST_F(TimerWheelTests, SingleTimer) {
        TimerWheel timerWheel;

        std::atomic<int> counter(0);

        timerWheel.AddTimer(std::make_shared<SingleShotTimer>(500ms, [&]() {
            counter.fetch_add(1);
        }));

        std::this_thread::sleep_for(1000ms);

        EXPECT_EQ(1, counter.load());
    }

    TEST_F(TimerWheelTests, TwoTimers) {
        TimerWheel timerWheel;

        std::atomic<int> counter(0);

        timerWheel.AddTimer(std::make_shared<SingleShotTimer>(100ms, [&]() {
            counter.fetch_add(1);
        }));
        timerWheel.AddTimer(std::make_shared<SingleShotTimer>(500ms, [&]() {
            counter.fetch_add(1);
        }));

        std::this_thread::sleep_for(1000ms);
        EXPECT_EQ(2, counter.load());
    }

    TEST_F(TimerWheelTests, ThreeTimers) {
        TimerWheel timerWheel;

        std::atomic<int> counter(0);

        timerWheel.AddTimer(std::make_shared<SingleShotTimer>(100ms, [&]() {
            counter.fetch_add(1);
        }));
        timerWheel.AddTimer(std::make_shared<SingleShotTimer>(500ms, [&]() {
            counter.fetch_add(1);
        }));
        timerWheel.AddTimer(std::make_shared<SingleShotTimer>(50ms, [&]() {
            counter.fetch_add(1);
        }));

        std::this_thread::sleep_for(1000ms);
        EXPECT_EQ(3, counter.load());
    }

    TEST_F(TimerWheelTests, TwoExpiredOneStillRunning) {
        TimerWheel timerWheel;

        std::atomic<int> counter(0);

        timerWheel.AddTimer(std::make_shared<SingleShotTimer>(100ms, [&]() {
            counter.fetch_add(1);
        }));
        timerWheel.AddTimer(std::make_shared<SingleShotTimer>(2000ms, [&]() {
            counter.fetch_add(1);
        }));
        timerWheel.AddTimer(std::make_shared<SingleShotTimer>(50ms, [&]() {
            counter.fetch_add(1);
        }));

        std::this_thread::sleep_for(1000ms);
        EXPECT_EQ(2, counter.load());
    }

    TEST_F(TimerWheelTests, ThreeRepeatsTimer) {
        TimerWheel timerWheel;

        std::atomic<int> counter(0);

        timerWheel.AddTimer(std::make_shared<RepetitiveTimer>(300ms, 3, [&]() {
            counter.fetch_add(1);
        }));

        std::this_thread::sleep_for(1000ms);

        EXPECT_EQ(3, counter.load());
    }

    TEST_F(TimerWheelTests, InfiniteRepeatsTimer) {
        TimerWheel timerWheel;

        std::atomic<int> counter(0);

        timerWheel.AddTimer(std::make_shared<RepetitiveTimer>(100ms, -1, [&]() {
            counter.fetch_add(1);
        }));

        std::this_thread::sleep_for(1050ms);

        EXPECT_EQ(10, counter.load());
    }

    TEST_F(TimerWheelTests, TwoRepetitiveTimers) {
        TimerWheel timerWheel;

        std::atomic<int> counter(0);

        timerWheel.AddTimer(std::make_shared<RepetitiveTimer>(300ms, 3, [&]() {
            counter.fetch_add(1);
        }));

        timerWheel.AddTimer(std::make_shared<RepetitiveTimer>(400ms, 2, [&]() {
            counter.fetch_add(1);
        }));

        std::this_thread::sleep_for(1000ms);

        EXPECT_EQ(5, counter.load());
    }

    TEST_F(TimerWheelTests, DurationExceptionTest) {
        TimerWheel timerWheel;

        // Provide a negative duration
        EXPECT_THROW(timerWheel.AddTimer(std::make_shared<RepetitiveTimer>(-100ms, 3, [&]() {})),
                     std::invalid_argument);
    }

    TEST_F(TimerWheelTests, FalseOnExpiryExceptionTest) {
        TimerWheel timerWheel;

        // Provide a std::function with no callable object
        EXPECT_THROW(timerWheel.AddTimer(std::make_shared<RepetitiveTimer>(-100ms, 3, std::function<void()>())),
                     std::invalid_argument);
    }

}  // namespace