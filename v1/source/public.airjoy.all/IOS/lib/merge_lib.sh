#!/bin/sh

lipo -create iPhoneOS/libairjoy.a iPhoneOS.armv7s/libairjoy.a iPhoneOS.Simulator/libairjoy.a -output libairjoy.a
