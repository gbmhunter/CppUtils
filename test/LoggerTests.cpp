///
/// \file 				LoggerTests.cpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2017-07-26
/// \last-modified		2018-01-22
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

    TEST_F(LoggerTests, BasicLogTest) {
        std::string savedMsg;
        Logger::Severity savedSeverity;
        Logger logger("TestLogger", Logger::Severity::DEBUG, Logger::Color::NONE, [&](Logger::Severity severity, std::string msg){
            savedMsg = msg;
            savedSeverity = severity;
        });

        LOG(logger, INFO, "Testing"); int lineNum = __LINE__;

        std::string filePath = __FILE__;

        std::cout << "savedMsg = " << savedMsg << std::endl;
        EXPECT_EQ(std::string() + "TestLogger (" + __FILE__ + ", " + std::to_string(lineNum) + ", TestBody()). INFO: Testing", savedMsg);
        EXPECT_EQ(Logger::Severity::INFO, savedSeverity);
    }

    TEST_F(LoggerTests, NoLogTest) {
        std::string savedMsg;        
        Logger logger("TestLogger", Logger::Severity::DEBUG, Logger::Color::NONE, [&](Logger::Severity severity, std::string msg){
            savedMsg = msg;
        });

        // Turn off logging
        logger.SetLogLevel(Logger::Severity::NONE);

        LOG(logger, INFO, "Testing");

        EXPECT_EQ("", savedMsg);
    }

    TEST_F(LoggerTests, ColourLogTest) {
        std::string savedMsg;
        Logger::Severity savedSeverity;
        Logger logger("TestLogger", Logger::Severity::DEBUG, Logger::Color::RED, [&](Logger::Severity severity, std::string msg){
            savedMsg = msg;
            savedSeverity = severity;
        });

        LOG(logger, INFO, "Testing"); int lineNum = __LINE__;

        std::string filePath = __FILE__;

        std::cout << "savedMsg = " << savedMsg << std::endl;
        EXPECT_EQ(std::string() + "\x1B[31mTestLogger (" + __FILE__ + ", " + std::to_string(lineNum) + ", TestBody()). INFO: Testing\x1B[0m", savedMsg);
        EXPECT_EQ(Logger::Severity::INFO, savedSeverity);
    }

    TEST_F(LoggerTests, ErrorColourTest) {
        std::string savedMsg;
        Logger::Severity savedSeverity;
        Logger logger("TestLogger", Logger::Severity::DEBUG, Logger::Color::PURPLE, [&](Logger::Severity severity, std::string msg){
            savedMsg = msg;
            savedSeverity = severity;
        });

        LOG(logger, ERROR, "Testing"); int lineNum = __LINE__;

        std::string filePath = __FILE__;

        std::cout << "savedMsg = " << savedMsg << std::endl;
        EXPECT_EQ(std::string() + "\x1B[31mTestLogger (" + __FILE__ + ", " + std::to_string(lineNum) + ", TestBody()). ERROR: Testing\x1B[0m", savedMsg);
        EXPECT_EQ(Logger::Severity::ERROR, savedSeverity);
    }

    TEST_F(LoggerTests, WarningColourTest) {
        std::string savedMsg;
        Logger::Severity savedSeverity;
        Logger logger("TestLogger", Logger::Severity::DEBUG, Logger::Color::PURPLE, [&](Logger::Severity severity, std::string msg){
            savedMsg = msg;
            savedSeverity = severity;
        });

        LOG(logger, WARNING, "Testing"); int lineNum = __LINE__;

        std::string filePath = __FILE__;

        std::cout << "savedMsg = " << savedMsg << std::endl;
        EXPECT_EQ(std::string() + "\x1B[33mTestLogger (" + __FILE__ + ", " + std::to_string(lineNum) + ", TestBody()). WARNING: Testing\x1B[0m", savedMsg);
        EXPECT_EQ(Logger::Severity::WARNING, savedSeverity);
    }

    
    TEST_F(LoggerTests, ReadmeExample) {
        Logger logger("ExampleLogger", Logger::Severity::DEBUG, Logger::Color::BLUE, [](Logger::Severity severity, std::string msg){
            std::cout << msg << std::endl;
        });

        LOG(logger, DEBUG, "Hello, world!");
    }

    TEST_F(LoggerTests, VAArgs) {
        std::string savedMsg;
        Logger::Severity savedSeverity;
        Logger logger("TestLogger", Logger::Severity::DEBUG, Logger::Color::NONE, [&](Logger::Severity severity, std::string msg){
            savedMsg = msg;
            savedSeverity = severity;
        });

        int myNum = 5;
        LOG(logger, INFO, "My num. = %i", myNum); int lineNum = __LINE__;

        std::string filePath = __FILE__;

        std::cout << "savedMsg = " << savedMsg << std::endl;
        EXPECT_EQ(std::string() + "TestLogger (" + __FILE__ + ", " + std::to_string(lineNum) + ", TestBody()). INFO: My num. = 5", savedMsg);
        EXPECT_EQ(Logger::Severity::INFO, savedSeverity);
    }
   
}  // namespace