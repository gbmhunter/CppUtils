///
/// \file 				SemaphoreTests.cpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2017-09-22
/// \last-modified		2017-09-22
/// \brief 				Contains tests for the Semaphore class.
/// \details
///		See README.md in root dir for more info.

// System includes
#include <chrono>

// 3rd party includes
#include <thread>
#include "gtest/gtest.h"

// User includes
#include "CppUtils/Semaphore.hpp"

using namespace mn::CppUtils;

namespace {

    class SemaphoreTests : public ::testing::Test {
    protected:
        SemaphoreTests() {}
        virtual ~SemaphoreTests() {}
    };

    TEST_F(SemaphoreTests, SingleThreadNotifyThenWait) {
        Semaphore semaphore;
        semaphore.Notify();
        semaphore.Wait();
    }

    TEST_F(SemaphoreTests, SingleThreadSleep) {
        Semaphore semaphore;

        auto start = std::chrono::high_resolution_clock::now();

        std::thread t1([&]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            semaphore.Notify();
        });

        semaphore.Wait();

        auto finish = std::chrono::high_resolution_clock::now();
        auto duration = finish - start;
        EXPECT_NEAR(1000, std::chrono::duration_cast<std::chrono::milliseconds>(duration).count(), 100);

        t1.join();
    }

    TEST_F(SemaphoreTests, TryWaitNoNotify) {
        Semaphore semaphore;
        bool wasNotify = semaphore.TryWait(std::chrono::milliseconds(100));
        EXPECT_FALSE(wasNotify);
    }

}  // namespace