///
/// \file 				ExceptionTests.cpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2017-11-09
/// \last-modified		2017-11-09
/// \brief 				Contains tests for the Exception class.
/// \details
///		See README.md in root dir for more info.

// System includes
// nothing

// 3rd party includes
#include "gtest/gtest.h"

// User includes
#include "CppUtils/Exception.hpp"

using namespace mn::CppUtils;

namespace {

    class ExceptionTests : public ::testing::Test {
    protected:

        ExceptionTests() {}
        virtual ~ExceptionTests() {}

    };

    TEST_F(ExceptionTests, MsgCheck) {

        std::string msg;
        try {
            THROW_EXCEPT("Testing");
        } catch(Exception e) {
            msg = e.what();
        }

        EXPECT_EQ(std::string() + __FILE__ + ":36: Testing", msg);
    }


}  // namespace