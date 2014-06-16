#!/bin/sh

rm -fr CMakeCache.txt
rm -fr CMakeFiles/
rm -fr cmake_install.cmake
rm -fr Makefile
rm -fr output/
rm -fr src/

cmake ../
make
