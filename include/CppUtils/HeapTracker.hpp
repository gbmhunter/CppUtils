///
/// \file 				HeapTracker.hpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2017-10-13
/// \last-modified		2017-10-13
/// \brief 				Contains the HeapTracker class.
/// \details
///		See README.md in root dir for more info.

#ifndef MN_CPP_UTILS_HEAP_TRACKER_H_
#define MN_CPP_UTILS_HEAP_TRACKER_H_

// System includes
#include <map>
#include <mutex>

namespace mn {
    namespace CppUtils {

        template<typename T>
        struct MemoryMapAllocator : std::allocator<T> {
            typedef typename std::allocator<T>::pointer pointer;
            typedef typename std::allocator<T>::size_type size_type;

            template<typename U>
            struct rebind {
                typedef MemoryMapAllocator<U> other;
            };

            MemoryMapAllocator() {}

            template<typename U>
            MemoryMapAllocator(MemoryMapAllocator<U> const &u)
                    :std::allocator<T>(u) {}

            pointer allocate(size_type size,
                             std::allocator<void>::const_pointer = 0) {

                void *p = std::malloc(size * sizeof(T));
                if (p == 0) {
                    throw std::bad_alloc();
                }
                return static_cast<pointer>(p);
            }

            void deallocate(pointer p, size_type) {
                std::free(p);
            }
        };

        class HeapTracker {
        public:

            using HeapMapType = std::map<void *, std::size_t, std::less<void *>, MemoryMapAllocator<std::pair<void *const, std::size_t>>>;

            static HeapTracker &Instance() {
                static HeapTracker heapTracker;
                return heapTracker;
            }

            void AddHeapAllocation(void *memoryAddress, std::size_t size) {
                std::unique_lock<std::mutex> lock(mutex_);
                HeapTracker::Instance().heapMap->insert(std::make_pair(memoryAddress, size));
                allocatedHeapMem_B += size;
            }

            void RemoveHeapAllocation(void *memoryAddress) {
                std::unique_lock<std::mutex> lock(mutex_);
                allocatedHeapMem_B -= (*HeapTracker::Instance().heapMap)[memoryAddress];
                HeapTracker::Instance().heapMap->erase(memoryAddress);
            }

            std::size_t GetHeapSize_B() {
                std::unique_lock<std::mutex> lock(mutex_);
                return allocatedHeapMem_B;
            }

        private:

            HeapMapType *heapMap;
            std::size_t allocatedHeapMem_B = 0;
            std::mutex mutex_;

            HeapTracker() {
                heapMap = new(std::malloc(
                        sizeof *heapMap)) std::map<void *, std::size_t, std::less<void *>, MemoryMapAllocator<std::pair<void *const, std::size_t>>>;
            }


        };

    } // namespace CppUtils
} // namespace mn


#define HEAP_TRACKER_NEW \
    void * operator new(std::size_t size) throw(std::bad_alloc) { \
        void * mem = std::malloc(size == 0 ? 1 : size); \
        if(mem == 0) { \
            throw std::bad_alloc(); \
        } \
        mn::CppUtils::HeapTracker::Instance().AddHeapAllocation(mem, size); \
        return mem; \
    }

#define HEAP_TRACKER_DELETE \
    void operator delete(void * mem) throw() { \
        std::free(mem); \
        mn::CppUtils::HeapTracker::Instance().RemoveHeapAllocation(mem); \
    }


#endif // MN_CPP_UTILS_HEAP_TRACKER_H_
