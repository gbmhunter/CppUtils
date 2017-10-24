///
/// \file 				MsgQueue.hpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2017-10-24
/// \last-modified		2017-10-24
/// \brief 				Contains the MsgQueue class.
/// \details
///		See README.md in root dir for more info.

#ifndef MN_CPP_UTILS_MSG_QUEUE_H_
#define MN_CPP_UTILS_MSG_QUEUE_H_

// System includes
#include <queue>
#include <mutex>
#include <future>
#include <condition_variable>

namespace mn {
    namespace CppUtils {
        namespace MsgQueue {

            using VData = std::shared_ptr<void>;

            class TxMsg {
            public:

                enum class ReturnType {
                    NO_RETURN_DATA,
                    RETURN_DATA
                };

                TxMsg(std::string id) {
                    id_ = id;
                }

                template<typename T>
                TxMsg(std::string id, T data) {
                    id_ = id;
                    data_ = std::static_pointer_cast<void>(data);
                }

                auto GetFuture() {
                    if (!promise_)
                        promise_ = std::make_shared<std::promise<VData>>();
                    return promise_->get_future();
                }

                void WaitForData() {

                }

                std::string id_;
                VData data_;
                std::shared_ptr<std::promise<VData>> promise_;
            };

            class RxMsg {
            public:
                RxMsg() {

                }

                VData GetData() {
                    if(!data_)
                        throw std::runtime_error("GetData() called but no data given.");

                    return data_;
                }

                void ReturnData(VData data) {
                    if (!promise_)
                        throw std::runtime_error("ReturnData called but no promise exists.");

                    promise_->set_value(data);
                }

                RxMsg& operator=(TxMsg rhs) {
                    id_ = rhs.id_;
                    data_ = rhs.data_;
                    promise_ = rhs.promise_;
                    return *this;
                }


                std::string id_;
                VData data_;
                std::shared_ptr<std::promise<VData>> promise_;
            };

            /// \brief       A thread-safe queue designed for inter-thread communication.
            class MsgQueue {
            public:

                /// \brief      Adds something to the back of the thread-safe queue.
                /// \details    This may be called from multiple threads at the "same time". Method
                ///             will block until item can be placed onto queue.
                void Push(const TxMsg &item) {
                    std::unique_lock<std::mutex> uniqueLock(mutex_);
                    queue_.push(item);
                    uniqueLock.unlock();
                    conditionVariable_.notify_one();
                }

                /// \brief      Adds something to the back of the thread-safe queue.
                /// \details    This may be called from multiple threads at the "same time". Method
                ///             will block until item can be placed onto queue.
                VData PushWait(TxMsg &item) {
                    std::unique_lock<std::mutex> uniqueLock(mutex_);


                    auto future = item.GetFuture();

                    // Push item onto queue
                    queue_.push(item);

                    uniqueLock.unlock();
                    conditionVariable_.notify_one();

                    future.wait();
                    return future.get();
                }

                /// \brief      Waits indefinitely until an item is available on the queue. Removes one item.
                /// \details    This may be called from multiple threads at the "same time". Method
                ///             will block indefinitely (no timeout) until there is an item on the queue.
                void Pop(RxMsg &item) {

                    // Lock the mutex
                    std::unique_lock<std::mutex> uniqueLock(mutex_);

                    conditionVariable_.wait(uniqueLock, [&] {
                        return !queue_.empty();
                    });

                    // If we get here, there is an item on the queue for us, and the lock has been taken out
                    auto txMsg = queue_.front();

                    // Copy (convert) TX msg to RX msg
                    item = txMsg;
                    queue_.pop();

                    // Mutex will automatically be unlocked here
                    // (as it is a unique_lock)
                }

                /// \brief      Removes one item from the front of the thread-safe queue.
                /// \details    This may be called from multiple threads at the "same time". Method
                ///             will block until the is an item on the queue OR a timeout occurs.
                /// \returns    Returns true is item received, returns false if a timeout occurred.
                bool TryPop(TxMsg &item, const std::chrono::milliseconds &timeout) {

                    // Lock the mutex
                    std::unique_lock<std::mutex> uniqueLock(mutex_);

                    if (!conditionVariable_.wait_for(uniqueLock, timeout, [&] {
                        return !queue_.empty();
                    })) {
                        return false;
                    }

                    // If we get here, there is an item on the queue for us, and the lock has been taken out
                    item = queue_.front();
                    queue_.pop();

                    // Mutex will automatically be unlocked here
                    // (as it is a unique_lock)
                    return true;
                }

                size_t Size() {
                    return queue_.size();
                }

            private:

                std::queue<TxMsg> queue_;
                std::mutex mutex_;
                std::condition_variable conditionVariable_;

            };
        } // namespace MsgQueue
    } // namespace CppUtils
} // namespace mn

#endif // #ifndef MN_CPP_UTILS_MSG_QUEUE_H_