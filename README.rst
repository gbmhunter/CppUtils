========
CppUtils
========

-----------------------------------------------------------------------------------------
A library with a range of utility classes and methods for jump starting C++ applications.
-----------------------------------------------------------------------------------------

.. image:: https://travis-ci.org/mbedded-ninja/CppUtils.svg?branch=master
	:target: https://travis-ci.org/mbedded-ninja/CppUtils

Bits.hpp
========

Contains static bit manipulation methods. This includes methods for generating a bit mask, getting an arbitrary sequence of bits from an integer type, or setting an arbitrary sequence of bits in an integer type.

**BitMask()**

.. code:: cpp

    uint8_t bitMask;
    bitMask = Bits::BitMask<uint8_t>(1));
    // bitMask = 0b00000001 or 0x01

    bitMask = Bits::BitMask<uint8_t>(4));
    // bitMask = 0b00001111 or 0x0F

    bitMask = Bits::BitMask<uint32_t>(11));
    // bitMask = 0x000007FF

**GetBits()**

.. code:: cpp

    uint8_t result;
    result = Bits::GetBits(0b11001100, 0, 3));
    // result = 0b00000100 or 0x4

    result = Bits::GetBits(0b11001100, 2, 5));
    // result = 0b00010011 or 0x13

**SetBits()**

.. code:: cpp

    uint8_t result;
    result = Bits::SetBits(0b00000000, 0b11011, 0, 5));
    // result = 0b00011011 or 0x1B

    result = Bits::SetBits(0b11111111, 0b11011, 0, 5));
    // result = 0b11111011 or 0xFB

Event.hpp
=========

Contains a basic event class which you can use to implement an event/listener based design.

**Basic Example**

.. code:: cpp

    Event<void()> event;

    event.AddListener([&](){
        std::cout << "Listener called!" << std::endl;
    });

    event.Fire(); // Prints "Listener called!"

**One Input Parameter**

.. code:: cpp

    Event<void(std::string)> event;

    event.AddListener([&](std::string msg){
        std::cout << msg << std::endl;
    });

    event.Fire("Hello"); // Prints "Hello"

**Bind To Method**

.. code:: cpp

    class TestClass {
        void TestMethod() {
            std::cout << "TestMethod() called." << std::endl;
        }
    }

    int main() {
        TestClass testClass;

        Event<void(std::string)> event;

        event.AddListener(std::bind(&TestClass::TestMethod, &testClass));

        event.Fire(); // Prints "TestMethod() called"
    }

Exception.hpp
=============

Exception.hpp provides functionality to throw an exception which will print the file name and line number that it was thrown on, which can be of great help when debugging!

Once including the header file, you can throw one of these exceptions with the :code:`THROW_EXCEPT()` macro, as shown in the below code:

.. code:: cpp

    #include "CppUtils/Exception.hpp"

    int main() {
        THROW_EXCEPT("Something bad happened!");
    }

The above code will print:

.. code:: cpp

    terminate called after throwing an instance of 'Exception'
      what(): /home/user/main.cpp:4: Something bad happened!


HeapTracker.hpp
===============

HeapTracker.hpp contains a :code:`HeapTracker` class which can be used keep track of memory allocations and deallocations (via :code:`new`, :code:`new[]`, :code:`delete`, and :code:`delete[]`) to the heap.

**Usage:**

.. code:: cpp

    #include "CppUtils/HeapTracker.hpp"

    // IMPORTANT! Only use these macros in one .cpp file. These macros define functions for new and delete
    // which override the default ones provided by the compiler. These new functions define some additional
    // code to keep track of new/delete calls.
    HEAP_TRACKER_NEW;
    HEAP_TRACKER_DELETE;

    int main() {
        std::cout << "Heap size (B) = " << HeapTracker::Instance().GetHeapSize_B() >> std::endl;
        // Prints some value "x"

        uint8_t* tempBuffer = new uint8_t[100];

        std::cout << "Heap size (B) = " << HeapTracker::Instance().GetHeapSize_B() >> std::endl;
        // Prints "x + 100"

        delete[] tempBuffer;

        std::cout << "Heap size (B) = " << HeapTracker::Instance().GetHeapSize_B() >> std::endl;
        // Prints "x" again

        return 0;
    }

**Points To Note:**

- :code:`HeapTracker` is thread safe. :code:`HeapTracker::Instance().GetHeapSize_B()` can be called from multiple threads. The additions to the overriden new and delete functions are also thread safe.
- :code:`HeapTracker` does not report the **exact** number of bytes used by the heap. This is becausethe compiler adds additional meta-data to a heap allocation so that :code:`delete` knows exactly how to delete the provided memory pointer (e.g. how many array elements to delete if :code:`delete[]` is called). Typically this is done by inserting a byte before the returned pointer which holds this meta-data, although this is implementation specific. Also, :code:`HeapTracker` creates a map on the heap to store needed pointer/size data which is not counted by :code:`GetHeapSize_B()` (a custom allocator is used to prevent infinite recursion). Thus the number returned by :code:`GetHeapSize_B()` will always be slightly less than the true heap usage, although should still be relatively accurate.
- As mentioned above, the macros :code:`HEAP_TRACKER_NEW` and :code:`HEAP_TRACKER_DELETE` should be included in only one source file. However, :code:`#include CppUtils/HeapTracker.hpp` can be included in as many files as you want.
- Using :code:`HeapTracker` imposes a **small** performance penalty as extra code is run on every call to :code:`new` or :code:`delete`. For every :code:`new`, a :code:`std::mutex` is locked, a map entry is created and the byte count incremented. On every :code:`delete`, a :code:`std::mutex` is locked, a map key/value pair is looked up and removed, and the byte count decremented.
- :code:`HeapTracker` is not able to keep track of heap allocations that do not use the standard :code:`new`/:code:`delete`. This includes any use of :code:`malloc()`/:code:`free()` and custom allocators.
- :code:`HeapTracker` is a thread-safe Singleton. Use :code:`HeapTracker::Instance()` to acquire a reference to the single instance.

StrConv.hpp
===========

A header-only collection of "to string" conversion methods which convert various objects (incl discrete values and iterable types) to various string representations.

**ToHex()**

.. code:: cpp

    std::cout << StrConv::ToHex(15, 2) << std::endl;
    // Prints "0xFF"

    std::cout << StrConv::ToHex(4, 3) << std::endl;
    // Prints "0x004"

    std::cout << StrConv::ToHex(std::vector({ 0x0A, 0x0B }) << std::endl;
    // Prints "{ 0x0A, 0x0B }"

**ToAscii()**

.. code:: cpp

    std::cout << StrConv::ToAscii(std::vector({ 'a', 'b' }) << std::endl;
    // Prints "{ 'a', 'b' }"


ThreadSafeQueue.hpp
===================

Contains a cross-platform thread safe queue object which uses the C++14 standard only (no UNIX :code:`pthread` or Windows :code:`CreateThread`).

Timer.hpp
=========

A timer which allows you to run code after a timeout occurs (code will be run within a new timer thread).

.. code:: cpp

    #include <atomic>
    #include "CppUtils/Timer.hpp"
    using namespace std::literals;

    std::atomic<bool> callbackCalled(false);
    Timer timer(100ms, [&]{
        // This will be called in the context of a timer thread, be aware
        // of concurrency concerns!
        callbackCalled.store(true);
    });

    std::this_thread::sleep_for(200ms);

    std::cout << "callbackCalled = " << callbackCalled.load() << std::endl;
    // Prints "true"

TimerWheel.hpp
==============

This header file contains a :code:`TimerWheel` class that can be used to easily create and manage timers (timeouts) in a multi-threaded environment.

**Single-Shot Timer Example**

.. code:: cpp

    #include "CppUtils/TimerWheel.hpp"

    using namespace std::literals;
    using namespace mn::CppUtils::TimerWheel;

    int main() {
        TimerWheel timerWheel;

        timerWheel.AddSingleShotTimer(500ms, [&]() {
            std::cout << "Timer expired!" << std::endl;
        });

        std::this_thread::sleep_for(1000ms);

        // "Timer expired!" will be printed after 500ms
    }

Note that the lambda callback provided above is executed in the context of the :code:`TimerWheel` thread. This callback could put messages onto other thread's commands queues, notify a :code:`std::condition_variable`, or lock a :code:`std::mutex` and perform actions on another thread's data.

:code:`TimerWheel` also supports *repetitive timers*.

**Repetitive Timer Example**

.. code:: cpp

    #include "CppUtils/TimerWheel.hpp"

    using namespace std::literals;
    using namespace mn::CppUtils::TimerWheel;

    int main() {
        TimerWheel timerWheel;

        timerWheel.AddRepetitiveTimer(300ms, 3, [&]() {
            std::cout << "Repeat" << std::endl;
        });

        std::this_thread::sleep_for(1000ms);

        // "Repeat" will be printed every 300ms for a total of three times.
        // Provide -1 instead of 3 to the timer constructor to make the timer run indefinitely.
    }