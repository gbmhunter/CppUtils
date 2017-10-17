///
/// \file 				TimerWheel.hpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2017-10-16
/// \last-modified		2017-10-16
/// \brief 				Contains the TimerWheel class.
/// \details
///		See README.md in root dir for more info.

#ifndef MN_CPP_UTILS_TIMER_WHEEL_H_
#define MN_CPP_UTILS_TIMER_WHEEL_H_

// System includes
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <deque>

// User includes
#include "ThreadSafeQueue.hpp"

namespace mn {
    namespace CppUtils {

        class WTimer {
        public:

            std::chrono::milliseconds duration_;
            std::chrono::high_resolution_clock::time_point startTime_;
            std::function<void()> onExpiry_;
        };

        /// \brief      A class that can be used to schedule timed operations.
        /// \details    Timer is stopped and timer thread joined on destruction.
        class TimerWheel {
        public:

            struct TimerWheelCmd {
                std::string name_;
                std::shared_ptr<void> data_;
            };

            TimerWheel() {
                thread_ = std::thread(&TimerWheel::Process, this);
            }

            ~TimerWheel() {
                if(thread_.joinable()) {
                    std::cout << "Sending EXIT command and joining TimerWheel thread." << std::endl;
                    TimerWheelCmd cmd;
                    cmd.name_ = "EXIT";
                    threadSafeQueue_.Push(cmd);
                    thread_.join();
                }
            }

            /// \note       Thread-safe and re-entrant.
            void AddTimer(std::chrono::milliseconds duration, std::function<void()> onExpiry) {
                auto heapTimer = std::make_shared<WTimer>();
                heapTimer->duration_ = duration;
                heapTimer->startTime_ = std::chrono::high_resolution_clock::now();
                heapTimer->onExpiry_ = onExpiry;

                TimerWheelCmd timerWheelCmd;
                timerWheelCmd.name_ = "ADD_TIMER";
                timerWheelCmd.data_ = heapTimer;
                threadSafeQueue_.Push(timerWheelCmd);
            }


        private:

            void Process() {

                std::cout << "Process called." << std::endl;
                std::chrono::milliseconds queueWaitTime;
                TimerWheelCmd timerWheelCmd;
                bool cmdReceived;

                while(true) {

                    // Check for exit condition
                    if(exit_)
                        return;

                    if(timers_.size() == 0) {
                        std::cout << "No timers present, waiting for command..." << std::endl;
                        threadSafeQueue_.Pop(timerWheelCmd);
                        cmdReceived = true;
                    } else {
                        // Calculate time to wait based on next timer to expire
                        if(CheckTimers(queueWaitTime)) {
                            std::cout << "Timers present, calling TryPop() with queueWaitTime = " << std::to_string(queueWaitTime.count()) << std::endl;
                            cmdReceived = threadSafeQueue_.TryPop(timerWheelCmd, queueWaitTime);
                        } else {
                            // No more timers left, so continue to next iteration of loop, where Pop() will be called.
                            continue;
                        }
                    }

                    currTime_ = std::chrono::high_resolution_clock::now();

                    if(cmdReceived) {
                        HandleCmd(timerWheelCmd);
                    }
                }
            }

            void HandleCmd(const TimerWheelCmd& cmd) {

                if(cmd.name_ == "ADD_TIMER") {
                    auto timerToInsert = std::static_pointer_cast<WTimer>(cmd.data_);


                    auto timerToInsertRemainingTime = currTime_ - timerToInsert->startTime_;


                    // Need to insert the new timer into the deque sorted on remaining time
                    for(auto it = timers_.begin(); it != timers_.end(); ++it) {

                        auto timer = *it;
                        auto remainingTime = currTime_ - timer->startTime_;

                        if(timerToInsertRemainingTime > remainingTime)
                            continue;
                        else {
                            // Current timer in queue has more remaining time, so insert new timer
                            // before this one!
                            timers_.insert(it, timerToInsert);
                            continue;
                        }
                    }

                    // If we get here, timer needs to be inserted at the end
                    timers_.push_back(timerToInsert);


                } else if(cmd.name_ == "EXIT") {
                    std::cout << "Received EXIT command." << std::endl;
                    exit_ = true;
                } else {
                    throw std::runtime_error("TimerWheel received unrecognized command.");
                }

            }

            bool CheckTimers(std::chrono::milliseconds& nextExpiry) {

                // Timers are sorted by expiry time (earliest expiry time is first)
                for(auto it = timers_.begin(); it != timers_.end();) {

                    auto timer = *it;
                    std::chrono::milliseconds remainingTime = std::chrono::duration_cast<std::chrono::milliseconds>(currTime_ - timer->startTime_);

                    if(remainingTime.count() <= 0) {
                        // Timer has expired!
                        timer->onExpiry_();
                        it = timers_.erase(it);
                        continue;
                    } else {
                        // Timer expiry is still in the future. Since the deque is sorted, this
                        // is the earliest expiring timer, so return the remaining time
                        nextExpiry = remainingTime;

                        // There is still a timer we need to wait for, so return true
                        return true;
                    }
                }

                // All timers must have been expired, and now no more exist!
                return false;
            }

            std::thread thread_;
            ThreadSafeQueue<TimerWheelCmd> threadSafeQueue_;
            std::deque<std::shared_ptr<WTimer>> timers_;

            /// \brief      This is updated once everytime the command queue is unblocked.
            std::chrono::high_resolution_clock::time_point currTime_;

            bool exit_ = false;

        };
    } // namespace CppUtils
} // namespace mn


#endif // MN_CPP_UTILS_TIMER_WHEEL_H_
