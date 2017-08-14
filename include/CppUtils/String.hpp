///
/// \file 				String.hpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2017-08-11
/// \last-modified		2017-08-11
/// \brief 				Contains the String class.
/// \details
///		See README.md in root dir for more info.

#ifndef MN_CPP_UTILS_STRING_H_
#define MN_CPP_UTILS_STRING_H_

// System includes
#include <cstdint>
#include <iomanip>
#include <sstream>

namespace mn {
    namespace CppUtils {

        class String {

        public:

            static std::string FromHex(uint32_t hexVal, uint8_t numHexChars) {
                std::stringstream output;
                output << "0x" << std::hex << std::uppercase << std::setw(numHexChars) << std::setfill('0') << hexVal;
                return output.str();
            }

        };
    } // namespace CppUtils
} // namespace mn


#endif // #ifndef MN_CPP_UTILS_STRING_H_