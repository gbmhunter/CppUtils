///
/// \file 				Bits.hpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2017-07-24
/// \last-modified		2017-08-11
/// \brief 				Contains the Bits class.
/// \details
///		See README.md in root dir for more info.

#ifndef MN_CPP_UTILS_BITS_H_
#define MN_CPP_UTILS_BITS_H_

// System includes
#include <climits>

namespace mn {
    namespace CppUtils {

        class Bits {

        public:

            /// \brief      Use to create a bitmask of a specified number of right-shifted 1's.
            /// \param      numOnes     The number of right-shifted 1's you want to create.
            /// \returns    A mask which has a number of right-shifted 1's.
            template <typename T>
            static constexpr T BitMask(uint8_t numOnes) {
                return static_cast<T>(-(numOnes != 0))
                       & (static_cast<T>(-1) >> ((sizeof(T) * CHAR_BIT) - numOnes));
            }

            /// \brief      Use to set an variable number of bits to a provided bit sequence in a 32-bit value
            /// \param      source          The source value to change bits on.
            /// \param      bitVals         The sequence of bits values to change.
            /// \param      startPosition   The bit position in source to start setting bits at.
            /// \param      numBits         The number of bits in source to set to new values.\
            /// \returns    The resultant variable, which is source with the bit values set at the correct position.
            template<typename T>
            static T SetBits(T source, T bitVals, uint8_t startPosition, uint8_t numBits) {
                // Make a mask, which is a sequence of numBits 1 in the correct start position
                // WARNING: Errors can occur if provided type to BitMask is signed
                T mask = BitMask<typename std::make_unsigned<T>::type>(numBits) << startPosition;

                return (source & ~mask) | ((bitVals << startPosition)&mask);
            }

            /// \brief      Use to extract a specific number of bits from the source variable, at the given start position.
            /// \param      source          The variable to extract bits from.
            /// \param      startPosition   The lowest bit position you want to start extracting bits from.
            /// \param      numBits         The number of bits you want to extract.
            /// \returns    The extracted bits, right-shifted.
            template <typename T>
            static T GetBits(T source, uint8_t startPosition, uint8_t numBits) {

                // Right-shift source down so that bits of interest are in LSB's of source
                source = source >> startPosition;

                // Mask to remove all bits of non-interest from left-hand side of source
                source &= BitMask<T>(numBits);

                return source;
            }
        };
    } // namespace CppUtils
} // namespace mn


#endif // #ifndef MN_CPP_UTILS_BITS_H_