#!/bin/sh

rm -fr CMakeCache.txt
rm -fr CMakeFiles/
rm -fr cmake_install.cmake
rm -fr Makefile
rm -fr output/
rm -fr src/

#export ARM_PATH=/usr/local/arm/4.3.2/bin
export ARM_PATH=/home/ouyang/CodeSourcery/Sourcery_G++_Lite/bin
export PATH=$PATH:$ARM_PATH

#export CC=/usr/local/arm/4.3.2/bin/arm-linux-gcc
#export AR=/usr/local/arm/4.3.2/bin/arm-linux-ar
#export RANLIB=/usr/local/arm/4.3.2/bin/arm-linux-ranlib

export CC=/home/ouyang/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-gcc
export AR=/home/ouyang/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-ar
export RANLIB=/home/ouyang/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-ranlib

cmake ../
make
