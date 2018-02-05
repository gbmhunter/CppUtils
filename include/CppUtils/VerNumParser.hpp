///
/// \file 				VerNumParser.hpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2018-02-04
/// \last-modified		2018-02-04
/// \brief 				Contains the VerNumParser class.
/// \details
///		See README.md in root dir for more info.

#ifndef MN_CPP_UTILS_VER_NUM_PARSER_H_
#define MN_CPP_UTILS_VER_NUM_PARSER_H_

// System includes
#include <string>
#include <vector>

namespace mn {
    namespace CppUtils {

        class VerNumParserEx : std::runtime_error {
        public:
            VerNumParserEx(std::string msg) : std::runtime_error(msg) {}
        };

        /// \details        Class that provides utility functions for dealing with string-based
        ///                     version numbers.
        class VerNumParser {
        public:

            /// \brief      Converts a version number in the string form "vX.X.X" into
            ///                 it's integer values for each section.
            /// \details    Supports an abritrary number of version sections, 
            ///                 e.g. "vX", "vX.X", "vX.X.X" are all supported.
            ///             First character may optionally be a "v", if not the
            ///                 version number must start straight away. 
            ///             Leading zeros on each version part are ignored.
            /// \returns    A vector of integers, each corresponding to each section of the
            ///                 version number, moving left to right. e.g. "vA.B.C" would store
            ///                 A as the first integer, B as the second, C as the last.
            /// \throws     VerNumParserEx if provided version number is ill-formed.
            static std::vector<uint32_t> Parse(const std::string& verNum) {

                std::vector<uint32_t> digits;
                std::string delimiter(".");

                size_t last = 0; size_t next = 0;

                // First char can optionally be "v" (otherwise version number starts here)
                if(verNum[0] == 'v')
                    last = 1;

                while ((next = verNum.find(delimiter, last)) != std::string::npos) {
                    std::string digitStr = verNum.substr(last, next-last);
                    // std::cout << digitStr << std::endl;

                    ParseDigit(digitStr, digits);               
                    last = next + 1;
                }
                std::string digitStr = verNum.substr(last, next-last);
                ParseDigit(digitStr, digits);
        
                return digits;
            }

            private:

                /// brief       Private helper function for #Parse().
                static void ParseDigit(const std::string& digitStr, std::vector<uint32_t>& digits) {
                    std::string::size_type nextChar;

                    int digit;
                    try {
                        digit = std::stoi(digitStr, &nextChar);
                    } catch(std::invalid_argument) {
                        throw VerNumParserEx("Version number was ill-formed.");
                    }

                    // Make sure there are no remaining characters in the string
                    if(nextChar != digitStr.size())
                        throw VerNumParserEx("Version number was ill-formed.");

                    if(digit < 0)
                        throw VerNumParserEx("Version number was ill-formed.");

                    digits.push_back((uint32_t)digit);
                }

        };
    } // namespace CppUtils
} // namespace mn


#endif // MN_CPP_UTILS_VER_NUM_PARSER_H_
