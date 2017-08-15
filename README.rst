========
CppUtils
========

-----------------------------------------------------------------------------------------
A library with a range of utility classes and methods for jump starting C++ applications.
-----------------------------------------------------------------------------------------

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


String.hpp
==========

**FromHex()**

.. code:: cpp

    std::cout << String::FromHex(15, 2);
    // Prints "0xFF"

    std::cout << String::FromHex(4, 3);
    // Prints "0x004"


ThreadSafeQ.hpp
===============

Contains a cross-platform thread safe queue object which uses the C++14 standard only (no UNIX :code:`pthread` or Windows :code:`CreateThread`).