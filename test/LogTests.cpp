///
/// \file 				LogTests.cpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2017-07-26
/// \last-modified		2017-08-10
/// \brief 				Contains tests for the logging macros.
/// \details
///		See README.md in root dir for more info.

// System includes
// nothing

// 3rd party includes
#include "gtest/gtest.h"

// User includes
#include "CppUtils/Log.hpp"

using namespace mn::CppUtils;

#define COLOR "\x1b[38;5;125m"

namespace {

    class LogTests : public ::testing::Test {
    protected:

        LogTests() {}
        virtual ~LogTests() {}

    };

    TEST_F(LogTests, BasicTest) {

        // It's hard to capture printf() output, let's just make
        // sure syntax compiles o.k.!
        LOG_INFO(COLOR, "%s", "Hello!\r\n");
    }

   
}  // namespace