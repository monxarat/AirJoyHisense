#!/bin/sh

rm -fr build
rm -fr objects

export ARM_PATH=/scratch/nathan/arm-lite/install/arm-none-linux-gnueabi/bin
export PATH=$PATH:$ARM_PATH

export CC=$ARM_PATH/arm-none-linux-gnueabi-gcc
export CXX=$ARM_PATH/arm-none-linux-gnueabi-c++
export AR=$ARM_PATH/arm-none-linux-gnueabi-ar
export RANLIB=$ARM_PATH/arm-none-linux-gnueabi-ranlib

make -f Makefile.arm os=linux
