///
/// \file 				Log.hpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2017-08-15
/// \last-modified		2018-01-22
/// \brief 				Contains Logging macros.
/// \details
///		See README.md in root dir for more info.

#ifndef MN_CPP_UTILS_LOG_H_
#define MN_CPP_UTILS_LOG_H_

// System includes
#include <climits>
#include <functional>
#include <stdarg.h>

namespace mn {
    namespace CppUtils {


/// \brief      This is the macro you should use in your code to log things!
/// \details    This will automatically capture the file name, line number and function name of LOG() by using preprocessor variables.
/// \param[in]  logger      The name of the logger object you want to use.
/// \param[in]  severity    The severity of the message.
/// \param[in]  msg         The message to log.
#define LOG(logger, severity, msg, ...) logger.MacroWillCall(msg, Logger::Severity::severity, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

#define config_TERM_ESCAPE_CODE				"\x1B["

#define config_TERM_TEXT_FORMAT_NORMAL 			config_TERM_ESCAPE_CODE "0m"	//!< Returns text to normal formatting. Widely supported.

#define config_TERM_TEXT_COLOUR_BLACK			config_TERM_ESCAPE_CODE "30m"	//!< Black text. Widely supported.
#define config_TERM_TEXT_COLOUR_LIGHT_RED 		config_TERM_ESCAPE_CODE "91m"	//!< Red text. Widely supported.
#define config_TERM_TEXT_COLOUR_LIGHT_GREEN 	config_TERM_ESCAPE_CODE "92m"	//!< Green text. Widely supported.
#define config_TERM_TEXT_COLOUR_LIGHT_YELLOW 	config_TERM_ESCAPE_CODE "93m"	//!< Yellow text. Widely supported.
#define config_TERM_TEXT_COLOUR_LIGHT_BLUE 		config_TERM_ESCAPE_CODE "94m"	//!< Blue text. Widely supported.
#define config_TERM_TEXT_COLOUR_LIGHT_PURPLE	config_TERM_ESCAPE_CODE "95m"	//!< Purple text. Widely supported.
#define config_TERM_TEXT_COLOUR_LIGHT_CYAN 		config_TERM_ESCAPE_CODE "96m"	//!< Cyan text. Widely supported.
#define config_TERM_TEXT_COLOUR_LIGHT_GREY		config_TERM_ESCAPE_CODE "37m"	//!< Light grey text. Widely supported.
#define config_TERM_TEXT_COLOUR_DEFAULT			config_TERM_ESCAPE_CODE "39m"	//!< default coloured text. Widely supported.

#define config_TERM_TEXT_COLOUR_DARK_GREY		config_TERM_ESCAPE_CODE "90m"	//!< Grey text. Widely supported.
#define config_TERM_TEXT_COLOUR_RED		        config_TERM_ESCAPE_CODE "31m"	//!< Red (light red, bold) text. Widely supported.
#define config_TERM_TEXT_COLOUR_GREEN 		config_TERM_ESCAPE_CODE "32m"	//!< Light green (bold) text. Widely supported.
#define config_TERM_TEXT_COLOUR_YELLOW 	config_TERM_ESCAPE_CODE "33m"	//!< Light yellow (bold) text. Widely supported.
#define config_TERM_TEXT_COLOUR_BLUE 		config_TERM_ESCAPE_CODE "34m"	//!< Light blue (bold) text. Widely supported.
#define config_TERM_TEXT_COLOUR_PURPLE		config_TERM_ESCAPE_CODE "35m"	//!< Light purple (bold) text. Widely supported.
#define config_TERM_TEXT_COLOUR_CYAN 		config_TERM_ESCAPE_CODE "36m"	//!< Light cyan (bold) text. Widely supported.
#define config_TERM_TEXT_COLOUR_WHITE 			config_TERM_ESCAPE_CODE "97m"	//!< White text. Widely supported.

        class Logger {

        public:
            enum class Severity {
                DEBUG,
                INFO,
                WARNING,
                ERROR,
                NONE // NONE has highest severity, so turns all printing off
            };

            enum class Color {
                NONE,
                RED,
                GREEN,
                YELLOW,
                BLUE,
                PURPLE,
                CYAN,
                CUSTOM
            };

            /// \brief      Constructor.
            Logger(std::string name, Severity logLevel, Color color, std::function<void(Severity, std::string)> output, uint32_t printfBufferSize_B = printfBufferSizeDefault_B) {
                name_ = name;
                logLevel_ = logLevel;
                normalColor_ = color;
                normalColor_ = color;
                output_ = output;

                warningColor_ = Color::YELLOW;
                errorColor_ = Color::RED;

                // Create buffer for printf (remember to delete in constructor)
                printfBuffer =  new char[printfBufferSize_B];
            }

            ~Logger() {
                delete[] printfBuffer;
            }

            /// \brief      This will be called by the LOG() macro defined above.
            inline void MacroWillCall(std::string msg, Severity severity, std::string fileName, int lineNum, std::string functionName, ...) {

                // Compare severities, only continue if message severity is higher or
                // equal to current log level
                if(severity < logLevel_)
                    return;

                std::string startColorText = "";
                std::string endColorText = "";
                if(normalColor_ != Color::NONE) {
                    // Predefined colours for warnings and errors
                    if(severity == Severity::WARNING) {
                        startColorText = GetColorString(warningColor_);
                        endColorText = config_TERM_TEXT_FORMAT_NORMAL;
                    } else if(severity == Severity::ERROR) {
                        startColorText = GetColorString(errorColor_);
                        endColorText = config_TERM_TEXT_FORMAT_NORMAL;
                    } else {
                        startColorText = GetColorString(normalColor_);
                        endColorText = config_TERM_TEXT_FORMAT_NORMAL;
                    }
                }

                va_list args;
                va_start(args, functionName);
                vsnprintf(printfBuffer, 100, msg.c_str(), args);

                msg = std::string(printfBuffer);

                std::string formattedMsg;
                formattedMsg += startColorText + name_ +
                        " (" + fileName + ", " + std::to_string(lineNum) + ", " + functionName + "()). " + ToString(severity) + ": " +
                        msg + endColorText;
                output_(severity, formattedMsg);
                va_end (args);
            }

            void SetLogLevel(Severity logLevel) {
                logLevel_ = logLevel;
            }

        private:


            std::string name_;
            Severity logLevel_;
            Logger::Color normalColor_;
            Logger::Color warningColor_;
            Logger::Color errorColor_;
            std::function<void(Severity, std::string)> output_;
            
            char* printfBuffer;
            static constexpr uint32_t printfBufferSizeDefault_B = 200;

            static std::string ToString(Severity severity) {
                switch(severity) {
                    case Severity::DEBUG:
                        return "DEBUG";
                    case Severity::INFO:
                        return "INFO";
                    case Severity::WARNING:
                        return "WARNING";
                    case Severity::ERROR:
                        return "ERROR";
                    default:
                        throw std::runtime_error("Severity not recognized.");

                }
            }

            static std::string GetColorString(Logger::Color color) {
                switch(color) {
                    case Color::NONE:
                        return "";
                    case Color::RED:
                        return config_TERM_TEXT_COLOUR_RED;
                    case Color::GREEN:
                        return config_TERM_TEXT_COLOUR_GREEN;
                    case Color::YELLOW:
                        return config_TERM_TEXT_COLOUR_YELLOW;
                    case Color::BLUE:
                        return config_TERM_TEXT_COLOUR_BLUE;
                    case Color::PURPLE:
                        return config_TERM_TEXT_COLOUR_PURPLE;
                    case Color::CYAN:
                        return config_TERM_TEXT_COLOUR_CYAN;
                    default:
                        throw std::runtime_error("Severity not recognized.");

                }

            }

        };

    } // namespace CppUtils
} // namespace mn


#endif // #ifndef MN_CPP_UTILS_LOG_H_