///
/// \file 				Log.hpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2017-08-15
/// \last-modified		2017-09-22
/// \brief 				Contains Logging macros.
/// \details
///		See README.md in root dir for more info.

#ifndef MN_CPP_UTILS_LOG_H_
#define MN_CPP_UTILS_LOG_H_

// System includes
#include <climits>

namespace mn {
    namespace CppUtils {

#define LOG_INFO(color, fmt, ...) \
        do { fprintf(stdout, "%s%s:%d:%s(): " fmt, color,  __FILE__, \
                                __LINE__, __func__, ##__VA_ARGS__); \
            fprintf(stdout, "\x1b[0m"); \
        } while (0)

    } // namespace CppUtils
} // namespace mn


#endif // #ifndef MN_CPP_UTILS_LOG_H_