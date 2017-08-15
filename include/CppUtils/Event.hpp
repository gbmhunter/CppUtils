///
/// \file 				Event.hpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2017-07-25
/// \last-modified		2017-08-14
/// \brief 				Contains the Event class.
/// \details
///		See README.md in root dir for more info.

#ifndef MN_CPP_UTILS_EVENT_H_
#define MN_CPP_UTILS_EVENT_H_

// System includes
#include <string>
#include <functional>

namespace mn {
    namespace CppUtils {

        template<typename T>
        class Event {

            using FuncType = T;

        public:

            /// \brief      Attach a listener to this event. Will be called when Event::Fire() is called.
            void AddListener(std::function<FuncType> listener) {
                listeners_.push_back(listener);
            };

            void RemoveListener(uint32_t index) {

                if(index >= listeners_.size())
                    throw std::invalid_argument("RemoveListener() called with index >= size of listeners_ array.");

                listeners_.erase(listeners_.begin() + index);
            }

            /// \brief      Remove all listeners attached to this event.
            void RemoveAllListeners() {
                listeners_.clear();
            }

            template <class ...Arg>
            void Fire(Arg... parameters) {
                for(auto listener : listeners_) {
                    // Perfect forward the parameters to the listener
                    listener(std::forward<Arg>(parameters)...);
                }
            }

        private:

            /// \brief      Keeps track of all the attached listeners.
            std::vector<std::function<T>> listeners_;

        };
    } // namespace CppUtils
} // namespace mn


#endif // MN_CPP_UTILS_EVENT_H_
