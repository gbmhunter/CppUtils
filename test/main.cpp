///
/// \file 				main.cpp
/// \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
/// \edited             n/a
/// \created			2017-07-24
/// \last-modified		2015-08-11
/// \brief 				Contains the main entry point for the unit test application.
/// \details
///		See README.md in root dir for more info.

// System includes
// nothing

// 3rd party includes
#include "gtest/gtest.h"

int main(int argc, char **argv) {

	::testing::InitGoogleTest(&argc, argv);
  	return RUN_ALL_TESTS();
}