========
CppUtils
========

-----------------------------------------------------------------------------------------
A library with a range of utility classes and methods for jump starting C++ applications.
-----------------------------------------------------------------------------------------

Bits.hpp
========

Contains static bit manipulation methods. This includes methods for generating a bit mask, getting an arbitrary sequence of bits from an integer type, or setting an arbitrary sequence of bits in an integer type.

String.hpp
==========

.. code:: cpp

    std::cout << String::FromHex(15, 2);
    // Prints "0xFF"

    std::cout << String::FromHex(4, 3);
    // Prints "0x004"


ThreadSafeQ.hpp
===============

Contains a cross-platform thread safe queue object which uses the C++14 standard only (no UNIX :code:`pthread` or Windows :code:`CreateThread`).