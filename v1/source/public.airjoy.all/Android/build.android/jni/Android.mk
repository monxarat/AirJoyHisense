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
# testAirPlayServer
#--------------------------------------------------------------------
#include $(CLEAR_VARS)

#SRC_DIR          := ../../testAirPlayServer
#LOCAL_MODULE     := testAirPlayServer
#LOCAL_SRC_FILES  := $(SRC_DIR)/testAirPlayServer.cpp

#LOCAL_C_INCLUDES := $(INC_ALL)
#LOCAL_CFLAGS     := -fexceptions         \
	                -frtti               \
	                -D__GLIBC__          \
                    -D__ANDROID__        \
                    -D__APPLE_BONJOUR__

#LOCAL_LDLIBS := -llog -L../lib -lairplay -ldns_sd_aj


#APP_CPPFLAGS     += -fexceptions
#APP_CPPFLAGS     += -frtti

#include $(BUILD_EXECUTABLE)


#--------------------------------------------------------------------
#
# LibJNIAnyplay 
# 
#--------------------------------------------------------------------
include $(CLEAR_VARS)

SRC_DIR          := ../../build.android/jni
LOCAL_MODULE     := jniAnyplay
LOCAL_SRC_FILES  := $(SRC_DIR)/JNIGlobalParam.cpp 			\
                    $(SRC_DIR)/AirplayJniListener.cpp		\
                    $(SRC_DIR)/APController.cpp				

LOCAL_C_INCLUDES := $(INC_ALL)
LOCAL_CFLAGS     := -fexceptions         \
	                -frtti               \
	                -D__GLIBC__          \
                    -D__ANDROID__        \
                    -D__APPLE_BONJOUR__

LOCAL_LDLIBS := -Lopenssl/libs -ldl_aj -lz -llog 
LOCAL_LDLIBS := -L../lib -lairplay -lairtunes -lairstream -ldns_sd_aj -lbonjour_aj -lz -llog

APP_CPPFLAGS     += -fexceptions
APP_CPPFLAGS     += -frtti

include $(BUILD_SHARED_LIBRARY)



##--------------------------------------------------------------------
##
## LibJNI AirJoy Server
## 
##--------------------------------------------------------------------
include $(CLEAR_VARS)

SRC_DIR          := ../../build.android/AJServer
LOCAL_MODULE     := AirJoyServer
LOCAL_SRC_FILES  := $(SRC_DIR)/AJDiscoveryListener.cpp 			\
                    $(SRC_DIR)/AJMediaListener.cpp				\
                    $(SRC_DIR)/AJNIChannel.cpp					\
                    $(SRC_DIR)/AJState.cpp						\
                    $(SRC_DIR)/AJServer.cpp				

LOCAL_C_INCLUDES := $(INC_ALL)
LOCAL_CFLAGS     := -fexceptions          \
	                -frtti                \
	                -D__GLIBC__           \
                    -D__ANDROID__         \
                    -D__APPLE_BONJOUR__

LOCAL_LDLIBS := -Lopenssl/libs -ldl_aj -lz -llog 
LOCAL_LDLIBS := -L../lib -lairjoy -lz -llog

APP_CPPFLAGS     += -fexceptions
APP_CPPFLAGS     += -frtti

include $(BUILD_SHARED_LIBRARY)


#--------------------------------------------------------------------
#
# LibJNI AirJoy Client
# 
#--------------------------------------------------------------------
include $(CLEAR_VARS)

SRC_DIR          := ../../build.android/AJClient
LOCAL_MODULE     := AirJoyClient
LOCAL_SRC_FILES  := $(SRC_DIR)/AJClientEntry.cpp 			\
                    $(SRC_DIR)/AJClient.cpp					\
                    $(SRC_DIR)/MediaPhoto.cpp				\
                    $(SRC_DIR)/MediaVideo.cpp				\
                    $(SRC_DIR)/MediaAudio.cpp				\
                    $(SRC_DIR)/AJMediaClientListener.cpp	\
					$(SRC_DIR)/AJDiscoveryListener.cpp		

LOCAL_C_INCLUDES := $(INC_ALL)
LOCAL_CFLAGS     := -fexceptions         \
	                -frtti               \
	                -D__GLIBC__          \
                    -D__ANDROID__        \
                    -D__APPLE_BONJOUR__

LOCAL_LDLIBS := -L../lib -lairjoy -ldns_sd_aj -lairstream -lz -llog

APP_CPPFLAGS     += -fexceptions
APP_CPPFLAGS     += -frtti

include $(BUILD_SHARED_LIBRARY)
