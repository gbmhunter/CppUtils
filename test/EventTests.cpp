///
/// \file 				EventTests.cpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2017-08-14
/// \last-modified		2017-08-14
/// \brief 				Contains tests for the Event class.
/// \details
///		See README.md in root dir for more info.

// System includes
// nothing

// 3rd party includes
#include "gtest/gtest.h"

// User includes
#include "CppUtils/Event.hpp"

using namespace mn::CppUtils;

namespace {

    class EventTests : public ::testing::Test {
    protected:

        EventTests() {}
        virtual ~EventTests() {}

    };

    TEST_F(EventTests, BasicLambda) {
        Event<void()> event;

        bool listenerCalled = false;
        event.AddListener([&](){
            listenerCalled = true;
        });

        event.Fire();
        EXPECT_EQ(true, listenerCalled);
    }

    class TestClass {
    public:
        bool methodCalled_ = false;
        void TestMethod() {
            methodCalled_ = true;
        }
    };

    TEST_F(EventTests, BasicClass) {
        Event<void()> event;
        TestClass testClass;
        event.AddListener(std::bind(&TestClass::TestMethod, &testClass));
        event.Fire();
        EXPECT_EQ(true, testClass.methodCalled_);
    }

}  // namespace