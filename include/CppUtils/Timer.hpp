///
/// \file 				Timer.hpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2017-10-06
/// \last-modified		2017-10-06
/// \brief 				Contains the Timer class.
/// \details
///		See README.md in root dir for more info.

#ifndef MN_CPP_UTILS_TIMER_H_
#define MN_CPP_UTILS_TIMER_H_

// System includes
#include <mutex>
#include <condition_variable>

#include "Semaphore.hpp"

namespace mn {
    namespace CppUtils {

        /// \brief      A class that can be used to schedule timed operations.
        /// \details    Timer is stopped and timer thread joined on destruction.
        class Timer {
        public:

            Timer(std::chrono::milliseconds duration, std::function<void()> callback) {

                duration_ = duration;
                callback_ = callback;

                t_ = std::thread([this]() {
                    if(semaphore_.TryWait(duration_)) {
                        // We got a notification, which means someone wishes to stop the timer
                        // before the timeout occured.
                        return;
                    } else {
                        // We waited and got no notification, which means the timer must of expired!
                        // That means we can call the callback
                        callback_();
                    }

                });
            }

            ~Timer() {
                // Tell the timer thread we wish to quit!
                semaphore_.Notify();
                if(t_.joinable())
                    t_.join();
            }


        private:
            std::thread t_;
            std::chrono::milliseconds duration_;
            std::function<void()> callback_;

            /// \brief      This semaphore is used to both block until timer timeout occurs, OR
            ///             to notify timer thread when timer is stopped (usually due to destructor being called).
            Semaphore semaphore_;
        };
    } // namespace CppUtils
} // namespace mn


#endif // MN_CPP_UTILS_TIMER_H_
