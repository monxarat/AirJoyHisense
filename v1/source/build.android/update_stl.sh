#!/bin/sh

# for ndk-r6, r5
#cp $ANDROID_NDK_PATH/sources/cxx-stl/gnu-libstdc++/libs/armeabi/libstdc++.a obj/local/armeabi/libstdc++.a

# for ndk-r8b
#cp $ANDROID_NDK_PATH/sources/cxx-stl/gnu-libstdc++/libs/armeabi/libgnustl_static.a obj/local/armeabi/libstdc++.a

# for ndk-r8b
#cp $ANDROID_NDK_PATH/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi/libgnustl_static.a obj/local/armeabi/libstdc++.a
#cp $ANDROID_NDK_PATH/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi-v7a/libgnustl_static.a obj/local/armeabi-v7a/libstdc++.a
#cp $ANDROID_NDK_PATH/sources/cxx-stl/gnu-libstdc++/4.6/libs/mips/libgnustl_static.a obj/local/mips/libstdc++.a
#cp $ANDROID_NDK_PATH/sources/cxx-stl/gnu-libstdc++/4.6/libs/x86/libgnustl_static.a obj/local/x86/libstdc++.a

# for ndk-r8b
cp $ANDROID_NDK_PATH/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi/libgnustl_static.a obj/local/armeabi/libstdc++.a
#cp $ANDROID_NDK_PATH/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi/libgnustl_static.a obj/local/armeabi/libgnustl_static.a
#cp $ANDROID_NDK_PATH/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi-v7a/libgnustl_static.a obj/local/armeabi-v7a/libgnustl_static.a
#cp $ANDROID_NDK_PATH/sources/cxx-stl/gnu-libstdc++/4.6/libs/mips/libgnustl_static.a obj/local/mips/libgnustl_static.a
#cp $ANDROID_NDK_PATH/sources/cxx-stl/gnu-libstdc++/4.6/libs/x86/libgnustl_static.a obj/local/x86/libgnustl_static.a
