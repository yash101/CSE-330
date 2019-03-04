#!/bin/bash

# HOME_PATH="$(dirname "${0}")"
HOME_PATH="$( cd "$(dirname "$0")"; pwd -P )"
BUILD_PATH=${HOME_PATH}/build_dir

rm -rf $BUILD_PATH
mkdir $BUILD_PATH

pushd ${BUILD_PATH} > /dev/null

cmake ..

if [[ $? -ne 0 ]] ;
then
	echo "Error: CMake build finished!"
	exit 1
fi

make -j 8

if [[ $? -ne 0 ]] ;
then
	echo "Error: build failed!"
	exit 1
fi

popd > /dev/null

