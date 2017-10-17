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
//#include "Semaphore.hpp"

namespace mn {
    namespace CppUtils {

        enum class WTimerType {
            SingleShot,
            Repeatitive
        };

        enum class WTimerState {
            Running,
            Expired
        };

        class WTimer {
        public:

            WTimerType type_;
            std::chrono::milliseconds duration_;
            std::chrono::high_resolution_clock::time_point startTime_;
            std::function<void()> onExpiry_;
            WTimerState state_;
        };

        /// \brief      A class that can be used to schedule timed operations.
        /// \details    Timer is stopped and timer thread joined on destruction.
        class TimerWheel {
        public:

            /// \brief      Creates a TimerWheel object and starts the timer wheel thread.
            TimerWheel() {
                thread_ = std::thread(&TimerWheel::Process, this);
            }

            /// \brief      Stops and joins with the timer wheel thread before destroying.
            ~TimerWheel() {
                if(thread_.joinable()) {
                    std::cout << "Sending EXIT command and joining TimerWheel thread." << std::endl;

                    //==============================================//
                    //============ START OF SYNC BLOCK =============//
                    //==============================================//
                    std::unique_lock<std::mutex> lock(mutex_);

                    exit_ = true;
                    wakeup_ = true;
                    lock.unlock();
                    //==============================================//
                    //============= END OF SYNC BLOCK ==============//
                    //==============================================//

                    std::cout << "Calling notify_one()..." << std::endl;
                    cv_.notify_one();
                    thread_.join();
                }
            }

            /// \brief      Use to add a new timer to the timer wheel.
            /// \returns    A pointer to the newly created timer.
            /// \note       Thread-safe and re-entrant.
            std::shared_ptr<WTimer> AddTimer(WTimerType type, std::chrono::milliseconds duration, std::function<void()> onExpiry) {
                std::cout << std::string() +__PRETTY_FUNCTION__ + " called.\n";
                auto newTimer = std::make_shared<WTimer>();
                newTimer->type_ = type;
                newTimer->duration_ = duration;
                newTimer->startTime_ = std::chrono::high_resolution_clock::now();
                newTimer->onExpiry_ = onExpiry;



                //==============================================//
                //============ START OF SYNC BLOCK =============//
                //==============================================//
                std::cout << "Waiting for lock...\n";
                std::unique_lock<std::mutex> lock(mutex_);
                std::cout << "Lock acquired.\n";

                InsertTimer(newTimer);

                wakeup_ = true;
                lock.unlock();
                //==============================================//
                //============= END OF SYNC BLOCK ==============//
                //==============================================//

                std::cout << "Calling notify_one()..." << std::endl;
                cv_.notify_one();

                return newTimer;
            }


        private:

            /// \brief      Function for the timer wheel thread.
            void Process() {
                std::cout << "Process called." << std::endl;
                std::unique_lock<std::mutex> lock(mutex_);
                std::chrono::milliseconds queueWaitTime;

                while(true) {

                    // Check for exit condition
                    if(exit_)
                        return;

                    wakeup_ = false;

                    CheckTimers(queueWaitTime);

                    if(timers_.size() == 0) {
                        std::cout << "No timers present, waiting for notify..." << std::endl;
                        while(!wakeup_)
                            cv_.wait(lock);
                        std::cout << "Notify received on TimerWheel thread." << std::endl;

                    } else {
                        // Calculate time to wait based on next timer to expire
                        std::cout << "Timers present, calling wait_for() with queueWaitTime (ms) = " << std::to_string(queueWaitTime.count()) << std::endl;
//                            cmdReceived = threadSafeQueue_.TryPop(timerWheelCmd, queueWaitTime);
//                            isNotify = semaphore_.TryWait(queueWaitTime);

                        cv_.wait_for(lock, queueWaitTime, [&]{
                            return wakeup_;
                        });
                        std::cout << "Notify/wakeup received on TimerWheel thread." << std::endl;
                    }
                }
            }

            /// \warning       Only call while mutex_ is locked.
            void InsertTimer(const std::shared_ptr<WTimer>& timerToInsert) {
                // Need to insert the new timer into the deque sorted on remaining time
                std::cout << "Inserting timer...\n";
                bool timerInserted = false;

                auto currTime = std::chrono::system_clock::now();
                auto newTimerRemainingTime = timerToInsert->duration_ - std::chrono::duration_cast<std::chrono::milliseconds>(currTime - timerToInsert->startTime_);

                for(auto it = timers_.begin(); it != timers_.end(); ++it) {
                    auto timer = *it;
                    auto remainingTime = timer->duration_ - std::chrono::duration_cast<std::chrono::milliseconds>(currTime - timer->startTime_);

                    if(newTimerRemainingTime > remainingTime)
                        continue;
                    else {
                        // Current timer in queue has more remaining time, so insert new timer
                        // before this one!
                        timers_.insert(it, timerToInsert);
                        timerInserted = true;
                        break;
                    }
                }

                // If we finish the iterator loop and the timer hasn't already been inserted,
                // the timer needs to be inserted at the end
                if(!timerInserted)
                    timers_.push_back(timerToInsert);
            }

            /// \brief      Checks all active timers. If any have expired, their onExpiry callbacks are called and then
            ///             the timer is removed.
            /// \param[out] nextExpiry  The earliest expiry duration from now for any of the active timers.
            /// \warning       Only call while mutex_ is locked.
            void CheckTimers(std::chrono::milliseconds& nextExpiry) {
                std::cout << "Checking timers. Num timers = " << timers_.size() << std::endl;

                auto currTime = std::chrono::system_clock::now();

                // Timers are sorted by expiry time (earliest expiry time is first)
                int count = 0;
                for(auto it = timers_.begin(); it != timers_.end();) {
                    std::cout << "Checking timer " << (*it).get() << "at position " << count << std::endl;
                    count++;

                    auto timer = *it;

                    std::cout << "timer->startTime = " << timer->startTime_.time_since_epoch().count() << std::endl;
                    std::cout << "currTime = " << currTime.time_since_epoch().count() << std::endl;
                    std::chrono::milliseconds durationSinceTimerStart = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - timer->startTime_);
                    std::cout << "durationSinceTimerStart (ms) = " << durationSinceTimerStart.count() << std::endl;

                    auto remainingTime = timer->duration_ - durationSinceTimerStart;
                    std::cout << "remainingTime (ms) = " << remainingTime.count() << std::endl;

                    if(remainingTime.count() <= 0) {
                        // Timer has expired!
                        std::cout << "Timer has expired." << std::endl;
                        timer->onExpiry_();
                        it = timers_.erase(it);

                        if(timer->type_ == WTimerType::Repeatitive) {
                            std::cout << "Timer is REPEATITIVE, re-adding...\n";
                            InsertTimer(timer);
                        }

                        continue;
                    } else {
                        std::cout << "Timer has NOT expired." << std::endl;
                        // Timer expiry is still in the future. Since the deque is sorted, this
                        // is the earliest expiring timer, so return the remaining time
                        nextExpiry = remainingTime;

                        // There is still a timer we need to wait for, so return true
                        return;
                    }
                }

                // All timers must have been expired, and now no more exist!
                return;
            }

            std::thread thread_;
            std::condition_variable cv_;
            std::mutex mutex_;
            bool wakeup_;

            std::deque<std::shared_ptr<WTimer>> timers_;

            bool exit_ = false;

        };
    } // namespace CppUtils
} // namespace mn


#endif // MN_CPP_UTILS_TIMER_WHEEL_H_
