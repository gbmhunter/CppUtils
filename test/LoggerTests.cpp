///
/// \file 				LoggerTests.cpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2017-07-26
/// \last-modified		2017-09-27
/// \brief 				Contains tests for the logging macros.
/// \details
///		See README.md in root dir for more info.

// System includes
#include <regex>

// 3rd party includes
#include "gtest/gtest.h"
#include "gmock/gmock.h"

// User includes
#include "CppUtils/Logger.hpp"

using namespace mn::CppUtils;

//#define COLOR "\x1b[38;5;125m"

namespace {

    class LoggerTests : public ::testing::Test {
    protected:

        LoggerTests() {}
        virtual ~LoggerTests() {}

    };

//    TEST_F(LoggerTests, BasicStringTest) {
//        // This uses no var args
//        LOG_INFO(COLOR, "Hello!\r\n");
//    }
//
//    TEST_F(LoggerTests, StringAsVarArg) {
//
//        // It's hard to capture printf() output, let's just make
//        // sure syntax compiles o.k.!
//        LOG_INFO(COLOR, "%s", "Hello!\r\n");
//    }

    TEST_F(LoggerTests, BasicLogTest) {
        std::string savedMsg;
        Logger logger("TestLogger", Logger::Severity::DEBUG, Logger::Color::NONE, [&](std::string msg){
            savedMsg = msg;
        });

        LOG(logger, INFO, "Testing"); int lineNum = __LINE__;

        std::string filePath = __FILE__;

        std::cout << "savedMsg = " << savedMsg << std::endl;
        EXPECT_EQ(std::string() + "TestLogger (" + __FILE__ + ", " + std::to_string(lineNum) + ", TestBody()). INFO: Testing", savedMsg);
    }

    TEST_F(LoggerTests, NoLogTest) {
        std::string savedMsg;
        Logger logger("TestLogger", Logger::Severity::DEBUG, Logger::Color::NONE, [&](std::string msg){
            savedMsg = msg;
        });

        // Turn off logging
        logger.SetLogLevel(Logger::Severity::NONE);

        LOG(logger, INFO, "Testing");

        EXPECT_EQ("", savedMsg);
    }

    TEST_F(LoggerTests, ColourLogTest) {
        std::string savedMsg;
        Logger logger("TestLogger", Logger::Severity::DEBUG, Logger::Color::RED, [&](std::string msg){
            savedMsg = msg;
        });

        LOG(logger, INFO, "Testing"); int lineNum = __LINE__;

        std::string filePath = __FILE__;

        std::cout << "savedMsg = " << savedMsg << std::endl;
        EXPECT_EQ(std::string() + "\x1B[31mTestLogger (" + __FILE__ + ", " + std::to_string(lineNum) + ", TestBody()). INFO: Testing\x1B[0m", savedMsg);
    }

    TEST_F(LoggerTests, ErrorColourTest) {
        std::string savedMsg;
        Logger logger("TestLogger", Logger::Severity::DEBUG, Logger::Color::PURPLE, [&](std::string msg){
            savedMsg = msg;
        });

        LOG(logger, ERROR, "Testing"); int lineNum = __LINE__;

        std::string filePath = __FILE__;

        std::cout << "savedMsg = " << savedMsg << std::endl;
        EXPECT_EQ(std::string() + "\x1B[31mTestLogger (" + __FILE__ + ", " + std::to_string(lineNum) + ", TestBody()). ERROR: Testing\x1B[0m", savedMsg);
    }

    TEST_F(LoggerTests, WarningColourTest) {
        std::string savedMsg;
        Logger logger("TestLogger", Logger::Severity::DEBUG, Logger::Color::PURPLE, [&](std::string msg){
            savedMsg = msg;
        });

        LOG(logger, WARNING, "Testing"); int lineNum = __LINE__;

        std::string filePath = __FILE__;

        std::cout << "savedMsg = " << savedMsg << std::endl;
        EXPECT_EQ(std::string() + "\x1B[33mTestLogger (" + __FILE__ + ", " + std::to_string(lineNum) + ", TestBody()). WARNING: Testing\x1B[0m", savedMsg);
    }

   
}  // namespace