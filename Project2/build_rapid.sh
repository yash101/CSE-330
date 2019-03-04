#!/bin/bash

# HOME_PATH="$(dirname "${0}")"
HOME_PATH="$( cd "$(dirname "$0")"; pwd -P )"
BUILD_PATH=${HOME_PATH}/build_dir


pushd ${BUILD_PATH} > /dev/null

make -j 8

if [[ $? -ne 0 ]] ;
then
	echo "Error: build failed!"
	exit 1
fi

popd > /dev/null

