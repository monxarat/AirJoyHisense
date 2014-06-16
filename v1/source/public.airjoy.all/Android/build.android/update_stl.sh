#!/bin/sh

# for ndk-r6, r5
#cp $ANDROID_NDK_PATH/sources/cxx-stl/gnu-libstdc++/libs/armeabi/libstdc++.a obj/local/armeabi/libstdc++.a

# for ndk-r8b
#cp $ANDROID_NDK_PATH/sources/cxx-stl/gnu-libstdc++/libs/armeabi/libgnustl_static.a obj/local/armeabi/libstdc++.a
#cp $ANDROID_NDK_PATH/sources/cxx-stl/gnu-libstdc++/4.4.3/libs/armeabi/libgnustl_static.a obj/local/armeabi/libstdc++.a

# for ndk-r8b
cp $ANDROID_NDK_PATH/sources/cxx-stl/gnu-libstdc++/4.4.3/libs/armeabi/libgnustl_static.a obj/local/armeabi/libstdc++.a
