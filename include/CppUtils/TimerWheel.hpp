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

            TimerWheel() {
                thread_ = std::thread(&TimerWheel::Process, this);
            }

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

            /// \note       Thread-safe and re-entrant.
            std::shared_ptr<WTimer> AddTimer(std::chrono::milliseconds duration, std::function<void()> onExpiry) {
                std::cout << std::string() +__PRETTY_FUNCTION__ + " called.\n";
                auto heapTimer = std::make_shared<WTimer>();
                heapTimer->duration_ = duration;
                heapTimer->startTime_ = std::chrono::high_resolution_clock::now();
                heapTimer->onExpiry_ = onExpiry;

                auto currTime = std::chrono::system_clock::now();
                auto timerToInsertRemainingTime = heapTimer->duration_ - std::chrono::duration_cast<std::chrono::milliseconds>(currTime - heapTimer->startTime_);

                //==============================================//
                //============ START OF SYNC BLOCK =============//
                //==============================================//
                std::cout << "Waiting for lock...\n";
                std::unique_lock<std::mutex> lock(mutex_);
                std::cout << "Lock acquired.\n";

                // Need to insert the new timer into the deque sorted on remaining time
                std::cout << "Inserting timer...\n";
                bool timerInserted = false;
                for(auto it = timers_.begin(); it != timers_.end(); ++it) {


                    auto timer = *it;
                    std::chrono::milliseconds durationSinceTimerStart = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - timer->startTime_);
                    auto remainingTime = timer->duration_ - durationSinceTimerStart;

                    if(timerToInsertRemainingTime > remainingTime)
                        continue;
                    else {
                        // Current timer in queue has more remaining time, so insert new timer
                        // before this one!
                        timers_.insert(it, heapTimer);
                        timerInserted = true;
                        break;
                    }
                }

                // If we finish the iterator loop and the timer hasn't already been inserted,
                // the timer needs to be inserted at the end
                if(!timerInserted)
                    timers_.push_back(heapTimer);

                wakeup_ = true;
                lock.unlock();
                //==============================================//
                //============= END OF SYNC BLOCK ==============//
                //==============================================//

                std::cout << "Calling notify_one()..." << std::endl;
                cv_.notify_one();

                return heapTimer;
            }


        private:

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


            bool CheckTimers(std::chrono::milliseconds& nextExpiry) {
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
                        continue;
                    } else {
                        std::cout << "Timer has NOT expired." << std::endl;
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
            std::condition_variable cv_;
            std::mutex mutex_;
            bool wakeup_;

            std::deque<std::shared_ptr<WTimer>> timers_;

            bool exit_ = false;

        };
    } // namespace CppUtils
} // namespace mn


#endif // MN_CPP_UTILS_TIMER_WHEEL_H_
