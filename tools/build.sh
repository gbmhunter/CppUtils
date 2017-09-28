#!/usr/bin/env bash

#
# \file 			build.sh
# \author 			Geoffrey Hunter (www.mbedded.ninja) <gbmhunter@gmail.com>
# \edited           n/a
# \created			2017-09-27
# \last-modified	2017-09-27
# \brief 			Bash script for building/installing the CppUtils headers.
# \details
#		See README.md in root dir for more info.

# Get script path
script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# 3rd party imports
. ${script_dir}/lib/shflags

# User imports
. ${script_dir}/lib/utilities.sh

printInfo "=========================================================================================="
printInfo "==================================== CppUtils build.sh ==================================="
printInfo "=========================================================================================="

set +e

# Define the command-line arguments
DEFINE_boolean 'install' 'false' 'Do you want to [i]nstall the CppUtils header files onto your local system after build?' 'i'

# parse the command-line
FLAGS "$@" || exit 1
eval set -- "${FLAGS_ARGV}"

# Any subsequent commands which fail will cause the shell script to exit immediately
# WARNING: Make sure to only activate this AFTER shflags has parsed command-line arguments
set -e

printInfo "install = ${FLAGS_install}"

BUILD_DIRECTORY_NAME="build"

# This will only make the build directory if it doesn't already
# exist. If it does exist, there is likely to be build artifacts
# in there already.
printInfo "Making and/or changing into build directory (${script_dir}/../${BUILD_DIRECTORY_NAME}/)..."
mkdir -p ${script_dir}/../${BUILD_DIRECTORY_NAME}/
cd ${script_dir}/../${BUILD_DIRECTORY_NAME}/

printInfo 'Invoking cmake...'
cmake ..

printInfo 'Invoking make...'
make -j8

printInfo 'Running unit tests...'
make -j8 run_unit_tests

if [[ "$FLAGS_install" == $FLAGS_TRUE ]]; then
	printInfo "Installing CppUtils headers onto local system..."
    sudo make install
fi
