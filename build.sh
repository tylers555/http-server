#!/bin/bash 

COMPILE_OPTIONS=-Wall\ -Wno-unused-value\ -Wno-unused-variable\ -Wno-unused-but-set-variable\ -Wno-unused-function
LINK_OPTIONS=
BUILD_MODE="debug"

INCLUDE_PATHS=-I"../source/"
LIBRARY_PATHS=

OUTPUT=LinuxSocketTestingDebug

pushd "build" > /dev/null

g++ ../source/linux/linux_main.cpp -g $COMPILE_OPTIONS $INCLUDE_PATHS -o$OUTPUT
# g++ ../source/linux/linux_main.cpp -g $COMPILE_OPTIONS $INCLUDE_PATHS -E -o$OUTPUT.p

popd > /dev/null