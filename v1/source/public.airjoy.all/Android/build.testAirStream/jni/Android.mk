#
# jxfengzi@gmail.com
#
# 2011-6-24
#
#
#

LOCAL_PATH := $(call my-dir)


INC_AIRJOY := ../include

INC_STL := $(ANDROID_NDK_PATH)/sources/cxx-stl/gnu-libstdc++/4.4.3/include               \
           $(ANDROID_NDK_PATH)/sources/cxx-stl/gnu-libstdc++/4.4.3/libs/armeabi/include

INC_ALL := $(INC_AIRJOY) \
           $(INC_STL)

#--------------------------------------------------------------------
# test
#--------------------------------------------------------------------
include $(CLEAR_VARS)

SRC_DIR          := ../src
LOCAL_MODULE     := test
LOCAL_SRC_FILES  := $(SRC_DIR)/testAirJoyClient.cpp

LOCAL_C_INCLUDES := $(INC_ALL)
LOCAL_CFLAGS     := -fexceptions         \
	                -frtti               \
	                -D__GLIBC__          \
                    -D__ANDROID__        \
                    -D__APPLE_BONJOUR__

LOCAL_LDLIBS := -llog -L../lib -lairstream

APP_CPPFLAGS     += -fexceptions
APP_CPPFLAGS     += -frtti

include $(BUILD_EXECUTABLE)
