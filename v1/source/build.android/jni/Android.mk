#
# jxfengzi@gmail.com
#
# 2011-6-24
#
#
#

LOCAL_PATH := $(call my-dir)


INC_3RDPART             := ../src/3rdpartInc
INC_AIR_COMMON          := ../src/AirCommon/include
INC_AIR_UTIL            := ../src/AirUtil/include
INC_AIR_SDP             := ../src/AirSdp/include
INC_AIR_NSD             := ../src/AirNsd/include
INC_AIR_UMP             := ../src/AirUmp/include
INC_AIRPLAY_COMMON      := ../src/AirJoyAll/AirPlayX/Common/include
INC_AIRPLAY_SERVER      := ../src/AirJoyAll/AirPlayX/AirPlay/server/include
INC_AIRTUNES_SERVER     := ../src/AirJoyAll/AirPlayX/AirTunes/server/include
INC_AIRPLAY_NSD         := ../src/AirJoyAll/AirPlayX/AirPlayNsd/include
INC_AIRJOY_SERVER       := ../src/AirJoyAll/AirJoyX/AirJoy/server/include
INC_AIRJOY_CLIENT       := ../src/AirJoyAll/AirJoyX/AirJoy/client/include
INC_AIRJOY_NSD          := ../src/AirJoyAll/AirJoyX/AirJoyNsd/include
INC_AIRSTREAM           := ../src/AirJoyAll/AirStreamX/AirFileServer/include
INC_PLIST               := ../src/3rdpartSrc/PlistCpp/include
INC_OPENSSL             := openssl/include

STL_INC := $(ANDROID_NDK_PATH)/sources/cxx-stl/gnu-libstdc++/4.6/include            \
           $(ANDROID_NDK_PATH)/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi/include

BOOST_INC := ../../../boost_1_49_0

INC_ALL := $(INC_3RDPART)               \
           $(INC_AIR_COMMON)           	\
           $(INC_AIR_UTIL)             	\
           $(INC_AIR_SDP)               \
           $(INC_AIR_NSD)               \
           $(INC_AIR_UMP)               \
           $(INC_AIRPLAY_COMMON)        \
           $(INC_AIRPLAY_SERVER)        \
           $(INC_AIRTUNES_SERVER)       \
           $(INC_AIRPLAY_NSD)           \
           $(INC_AIRJOY_SERVER)	        \
           $(INC_AIRJOY_CLIENT)	        \
           $(INC_AIRJOY_NSD)	        \
           $(INC_AIRSTREAM)             \
           $(INC_PLIST)                 \
           $(BOOST_INC)                 \
           $(STL_INC)


#--------------------------------------------------------------------
# AirCommon
#--------------------------------------------------------------------
include $(CLEAR_VARS)

SRC_DIR          := ../../src/AirCommon/src
LOCAL_MODULE     := libaircommon_static
LOCAL_SRC_FILES  := $(SRC_DIR)/BoostSystemPrebuild.cpp      \
                    $(SRC_DIR)/BoostFileSystemPrebuild.cpp  \
                    $(SRC_DIR)/BoostDateTimePrebuild.cpp    \
                    $(SRC_DIR)/BoostThreadPrebuild.cpp      \
                    $(SRC_DIR)/HttpMessage.cpp				\
                    $(SRC_DIR)/HttpMessageType.cpp			\
                    $(SRC_DIR)/HttpTextParam.cpp			\
                    $(SRC_DIR)/UmpMessage.cpp
LOCAL_C_INCLUDES := $(INC_ALL)
LOCAL_CFLAGS     := -fexceptions         \
	                -frtti               \
	                -fPIC                \
	                -D__GLIBC__          \
                    -D__ANDROID__        \
                    -D__APPLE_BONJOUR__

APP_CPPFLAGS += -fexceptions
APP_CPPFLAGS += -frtti

include $(BUILD_STATIC_LIBRARY)


#--------------------------------------------------------------------
# AirUtil
#--------------------------------------------------------------------
include $(CLEAR_VARS)

SRC_DIR          := ../../src/AirUtil/src
LOCAL_MODULE     := libairutil_static
LOCAL_SRC_FILES  := $(SRC_DIR)/UtilBase64.cpp       \
                    $(SRC_DIR)/UtilUtf8.cpp         \
                    $(SRC_DIR)/UtilItoa.cpp         \
                    $(SRC_DIR)/UtilMd5.cpp          \
                    $(SRC_DIR)/UtilType.cpp         \
                    $(SRC_DIR)/UtilString.cpp
LOCAL_C_INCLUDES := $(INC_ALL)
LOCAL_CFLAGS     := -fexceptions         \
	                -frtti               \
	                -fPIC                \
	                -D__GLIBC__          \
                    -D__ANDROID__        \
                    -D__APPLE_BONJOUR__

APP_CPPFLAGS += -fexceptions
APP_CPPFLAGS += -frtti

include $(BUILD_STATIC_LIBRARY)

#--------------------------------------------------------------------
# AirSdp
#--------------------------------------------------------------------
include $(CLEAR_VARS)

SRC_DIR          := ../../src/AirSdp/src
LOCAL_MODULE     := libairsdp_static
LOCAL_SRC_FILES  := $(SRC_DIR)/SdpDiscoveryWorker.cpp        \
                    $(SRC_DIR)/SdpPublishWorker.cpp          \
                    $(SRC_DIR)/SdpNetwork.cpp
LOCAL_C_INCLUDES := $(INC_ALL)
LOCAL_CFLAGS     := -fexceptions         \
	                -frtti               \
	                -fPIC                \
	                -D__GLIBC__          \
                    -D__ANDROID__        \
                    -D__APPLE_BONJOUR__

APP_CPPFLAGS += -fexceptions
APP_CPPFLAGS += -frtti

include $(BUILD_STATIC_LIBRARY)


#--------------------------------------------------------------------
# AirUmp
#--------------------------------------------------------------------
include $(CLEAR_VARS)

SRC_DIR          := ../../src/AirUmp/src
LOCAL_MODULE     := libairump_static
LOCAL_SRC_FILES  := $(SRC_DIR)/TcpClient.cpp       \
                    $(SRC_DIR)/TcpSession.cpp      \
                    $(SRC_DIR)/UmpClient.cpp       \
                    $(SRC_DIR)/UmpServer.cpp       \
                    $(SRC_DIR)/Ump.cpp
LOCAL_C_INCLUDES := $(INC_ALL)
LOCAL_CFLAGS     := -fexceptions         \
	                -frtti               \
	                -fPIC                \
	                -D__GLIBC__          \
                    -D__ANDROID__        \
                    -D__APPLE_BONJOUR__

APP_CPPFLAGS += -fexceptions
APP_CPPFLAGS += -frtti

include $(BUILD_STATIC_LIBRARY)


#--------------------------------------------------------------------
# Plist 
#--------------------------------------------------------------------
include $(CLEAR_VARS)

SRC_DIR          := ../../src/3rdpartSrc/PlistCpp/src
LOCAL_MODULE     := libplist_static
LOCAL_SRC_FILES  := $(SRC_DIR)/pugixml.cpp
LOCAL_C_INCLUDES := $(INC_ALL)
LOCAL_CFLAGS     := -fexceptions         \
	                -frtti               \
	                -fPIC                \
	                -D__GLIBC__          \
                    -D__ANDROID__        \
                    -D__APPLE_BONJOUR__

APP_CPPFLAGS += -fexceptions
APP_CPPFLAGS += -frtti

include $(BUILD_STATIC_LIBRARY)


#--------------------------------------------------------------------
# AirNsd
#--------------------------------------------------------------------
include $(CLEAR_VARS)

SRC_DIR          := ../../src/AirNsd/src
LOCAL_MODULE     := libairnsd_static
LOCAL_SRC_FILES  := $(SRC_DIR)/Nsd.cpp

LOCAL_C_INCLUDES := $(INC_ALL)           \
                    $(INC_AIR_NSD)

LOCAL_CFLAGS     := -fexceptions         \
	                -frtti               \
	                -fPIC                \
	                -D__GLIBC__          \
                    -D__ANDROID__        \
                    -D__APPLE_BONJOUR__

APP_CPPFLAGS += -fexceptions
APP_CPPFLAGS += -frtti

include $(BUILD_STATIC_LIBRARY)


#--------------------------------------------------------------------
# AirPlay
#--------------------------------------------------------------------
include $(CLEAR_VARS)

SRC_DIR          := ../../src/AirJoyAll/AirPlayX
LOCAL_MODULE     := libairplay

AIRPLAY_COMMON_SRC  := $(SRC_DIR)/Common/src/AirPlayServiceInfo.cpp             \
                       $(SRC_DIR)/Common/src/AirTunesServiceInfo.cpp
 
AIRPLAY_NSD_SRC  := $(SRC_DIR)/AirPlayNsd/src/AirPlayNsd.cpp                  	\
                    $(SRC_DIR)/AirPlayNsd/src/AirPlayNsdCore.cpp         	    \
                    $(SRC_DIR)/AirPlayNsd/src/AirPlayNsdDiscoveryHandler.cpp    \
                    $(SRC_DIR)/AirPlayNsd/src/AirPlayNsdPublishHandler.cpp
 
AIRPLAY_SERVER_SRC  := $(SRC_DIR)/AirPlay/server/src/AirPlayServer.cpp          \
                       $(SRC_DIR)/AirPlay/server/src/AirPlayServerCore.cpp      \
                       $(SRC_DIR)/AirPlay/server/src/AirPlayServerHandler.cpp   \
                       $(SRC_DIR)/AirPlay/server/src/AirPlayEvent.cpp

LOCAL_SRC_FILES  := $(AIRPLAY_COMMON_SRC) \
                    $(AIRPLAY_SERVER_SRC)  \
                    $(AIRPLAY_NSD_SRC)

LOCAL_C_INCLUDES := $(INC_ALL)           \
                    $(INC_AIR_NSD)

LOCAL_CFLAGS     := -fexceptions         \
	                -frtti               \
	                -fPIC                \
	                -D__GLIBC__          \
                    -D__ANDROID__        \
                    -D__APPLE_BONJOUR__

APP_CPPFLAGS += -fexceptions
APP_CPPFLAGS += -frtti
APP_STL := gnustl_static

LOCAL_STATIC_LIBRARIES := libaircommon_static       \
                          libairutil_static         \
                          libairnsd_static          \
                          libairump_static			\
                          libplist_static

LOCAL_LDLIBS := -llog -Lmdns/ -ldns_sd_aj
 
include $(BUILD_SHARED_LIBRARY)


#--------------------------------------------------------------------
# AirTunes
#--------------------------------------------------------------------
include $(CLEAR_VARS)

SRC_DIR          := ../../src/AirJoyAll/AirPlayX
LOCAL_MODULE     := libairtunes

AIRTUNES_COMMON_SRC  := $(SRC_DIR)/Common/src/AirPlayServiceInfo.cpp             \
                        $(SRC_DIR)/Common/src/AirTunesServiceInfo.cpp
 
AIRTUNES_NSD_SRC  := $(SRC_DIR)/AirPlayNsd/src/AirPlayNsd.cpp                  	 \
                     $(SRC_DIR)/AirPlayNsd/src/AirPlayNsdCore.cpp         	     \
                     $(SRC_DIR)/AirPlayNsd/src/AirPlayNsdDiscoveryHandler.cpp    \
                     $(SRC_DIR)/AirPlayNsd/src/AirPlayNsdPublishHandler.cpp
 
AIRTUNES_SERVER_SRC  := $(SRC_DIR)/AirTunes/server/src/AirTunesServer.cpp          \
                        $(SRC_DIR)/AirTunes/server/src/AirTunesServerCore.cpp      \
                        $(SRC_DIR)/AirTunes/server/src/AirTunesServerHandler.cpp   \
                        $(SRC_DIR)/AirTunes/server/src/AirCrypto.cpp               \
                        $(SRC_DIR)/AirTunes/server/src/AirBase64.cpp               \
                        $(SRC_DIR)/AirTunes/server/src/RtspMessage.cpp             \
                        $(SRC_DIR)/AirTunes/server/src/AirPort.cpp                 \
                        $(SRC_DIR)/AirTunes/server/src/AirPortControlHandler.cpp   \
                        $(SRC_DIR)/AirTunes/server/src/AirPortDataHandler.cpp      \
                        $(SRC_DIR)/AirTunes/server/src/AirPortTimingHandler.cpp    \
                        $(SRC_DIR)/AirTunes/server/src/dmap_parser.cpp             \
                        $(SRC_DIR)/AirTunes/server/src/Dmap.cpp                    \
                        $(SRC_DIR)/AirTunes/server/src/AudioBuffer.cpp             \
                        $(SRC_DIR)/AirTunes/server/src/AudioDriver.cpp             \
                        $(SRC_DIR)/AirTunes/server/src/AudioPlayer.cpp             \
                        $(SRC_DIR)/AirTunes/server/src/DacpClient.cpp              \
                        $(SRC_DIR)/AirTunes/server/src/DacpClientCore.cpp          \
                        $(SRC_DIR)/AirTunes/server/src/DacpClientHandler.cpp


ALAC_SRC  := $(SRC_DIR)/AirTunes/server/src/alac.cpp
 

LOCAL_SRC_FILES  := $(AIRTUNES_COMMON_SRC)  \
                    $(AIRTUNES_SERVER_SRC)  \
                    $(AIRTUNES_NSD_SRC)     \
                    $(ALAC_SRC)

LOCAL_C_INCLUDES := $(INC_ALL)           \
                    $(INC_AIR_NSD)

LOCAL_CFLAGS     := -fexceptions         \
	                -frtti               \
	                -fPIC                \
	                -D__GLIBC__          \
                    -D__ANDROID__        \
                    -D__APPLE_BONJOUR__

APP_CPPFLAGS += -fexceptions
APP_CPPFLAGS += -frtti
APP_STL := gnustl_static

LOCAL_STATIC_LIBRARIES := libaircommon_static       \
                          libairutil_static         \
                          libairnsd_static          \
                          libairump_static			\
                          libplist_static

#LOCAL_LDLIBS := -llog -Lmdns/ -ldns_sd -Lopenssl/libs -lcrypto -lssl -lao
LOCAL_LDLIBS := -llog -Lmdns/ -ldns_sd_aj -Lopenssl/libs -lcrypto_aj -lssl_aj -lz -ldl_aj
 
include $(BUILD_SHARED_LIBRARY)


#--------------------------------------------------------------------
# AirJoy
#--------------------------------------------------------------------
include $(CLEAR_VARS)

SRC_DIR          := ../../src/AirJoyAll/AirJoyX
LOCAL_MODULE     := libairjoy

AIRJOY_NSD_SRC  := $(SRC_DIR)/AirJoyNsd/src/AirJoyNsd.cpp                  	    \
                   $(SRC_DIR)/AirJoyNsd/src/AirJoyNsdCore.cpp         	    	\
                   $(SRC_DIR)/AirJoyNsd/src/AirJoyNsdDiscoveryHandler.cpp    	\
                   $(SRC_DIR)/AirJoyNsd/src/AirJoyNsdPublishHandler.cpp
 
AIRJOY_SERVER_SRC  := $(SRC_DIR)/AirJoy/server/src/AirJoyDefault.cpp			\
                      $(SRC_DIR)/AirJoy/server/src/AirJoyServer.cpp				\
                      $(SRC_DIR)/AirJoy/server/src/AirJoyServerCore.cpp			\
                      $(SRC_DIR)/AirJoy/server/src/AirJoyServerHandler.cpp		\
                      $(SRC_DIR)/AirJoy/server/src/AirQueryHandler.cpp			\
                      $(SRC_DIR)/AirJoy/server/src/AirFileHandler.cpp			\
                      $(SRC_DIR)/AirJoy/server/src/AirMediaHandler.cpp          \
                      $(SRC_DIR)/AirJoy/server/src/AirMediaEvent.cpp

AIRJOY_CLIENT_SRC   := $(SRC_DIR)/AirJoy/client/src/AirJoyClient.cpp            \
                       $(SRC_DIR)/AirJoy/client/src/AirJoyClientCore.cpp		\
                       $(SRC_DIR)/AirJoy/client/src/AirMediaClient.cpp			\
                       $(SRC_DIR)/AirJoy/client/src/AirMediaClientPhoto.cpp		\
                       $(SRC_DIR)/AirJoy/client/src/AirMediaClientVideo.cpp		\
                       $(SRC_DIR)/AirJoy/client/src/AirMediaClientAudio.cpp		\
                       $(SRC_DIR)/AirJoy/client/src/AirMediaClientVolume.cpp	\
                       $(SRC_DIR)/AirJoy/client/src/AirMediaClientQueryHandler.cpp

LOCAL_SRC_FILES  := $(AIRJOY_SERVER_SRC)  	\
                    $(AIRJOY_CLIENT_SRC)	\
                    $(AIRJOY_NSD_SRC)

LOCAL_C_INCLUDES := $(INC_ALL)

LOCAL_CFLAGS     := -fexceptions         \
	                -frtti               \
	                -fPIC                \
	                -D__GLIBC__          \
                    -D__ANDROID__        \
                    -D__APPLE_BONJOUR__

APP_CPPFLAGS += -fexceptions
APP_CPPFLAGS += -frtti
APP_STL := gnustl_static

LOCAL_STATIC_LIBRARIES := libaircommon_static       \
                          libairutil_static         \
                          libairsdp_static          \
                          libairump_static

LOCAL_LDLIBS := -llog
 
include $(BUILD_SHARED_LIBRARY)

#--------------------------------------------------------------------
# AirStream
#--------------------------------------------------------------------
include $(CLEAR_VARS)

SRC_DIR          := ../../src/AirJoyAll/AirStreamX
LOCAL_MODULE     := libairstream

LOCAL_SRC_FILES  := $(SRC_DIR)/AirFileServer/src/AirFileServer.cpp  \
                    $(SRC_DIR)/AirFileServer/src/AirHttpServer.cpp  \
                    $(SRC_DIR)/AirFileServer/src/mongoose.cpp

LOCAL_C_INCLUDES := $(INC_ALL)

LOCAL_CFLAGS     := -fexceptions         \
	                -frtti               \
	                -fPIC                \
	                -D__GLIBC__          \
                    -D__ANDROID__        \
                    -D__APPLE_BONJOUR__

APP_CPPFLAGS += -fexceptions
APP_CPPFLAGS += -frtti
APP_STL := gnustl_static

LOCAL_STATIC_LIBRARIES := libaircommon_static       \
                          libairutil_static

LOCAL_LDLIBS := -llog
 
include $(BUILD_SHARED_LIBRARY)

#--------------------------------------------------------------------
# testAirPlayServer
#--------------------------------------------------------------------
include $(CLEAR_VARS)

SRC_DIR          := ../../src/AirJoyAll/AirPlayX/testAirPlayServer
LOCAL_MODULE     := testAirPlayServer
LOCAL_SRC_FILES  := $(SRC_DIR)/testAirPlayServer.cpp

LOCAL_C_INCLUDES := $(INC_ALL)
LOCAL_CFLAGS     := -fexceptions         \
	                -frtti               \
	                -D__GLIBC__          \
                    -D__ANDROID__        \
                    -D__APPLE_BONJOUR__

LOCAL_SHARED_LIBRARIES := airplay
LOCAL_LDLIBS := -llog -Lmdns/ -ldns_sd_aj

APP_CPPFLAGS     += -fexceptions
APP_CPPFLAGS     += -frtti

include $(BUILD_EXECUTABLE)

#--------------------------------------------------------------------
# testAirTunesServer
#--------------------------------------------------------------------
include $(CLEAR_VARS)

SRC_DIR          := ../../src/AirJoyAll/AirPlayX/testAirTunesServer
LOCAL_MODULE     := testAirTunesServer
LOCAL_SRC_FILES  := $(SRC_DIR)/testAirTunesServer.cpp

LOCAL_C_INCLUDES := $(INC_ALL)
LOCAL_CFLAGS     := -fexceptions         \
	                -frtti               \
	                -D__GLIBC__          \
                    -D__ANDROID__        \
                    -D__APPLE_BONJOUR__

LOCAL_SHARED_LIBRARIES := airtunes
LOCAL_LDLIBS := -ldl_aj -llog -Lmdns/ -ldns_sd_aj -Lopenssl/libs -lcrypto_aj -lssl_aj -lz

APP_CPPFLAGS     += -fexceptions
APP_CPPFLAGS     += -frtti

include $(BUILD_EXECUTABLE)


#--------------------------------------------------------------------
# testAirJoyServer
#--------------------------------------------------------------------
include $(CLEAR_VARS)

SRC_DIR          := ../../src/AirJoyAll/AirJoyX/test
LOCAL_MODULE     := testAirJoyServer
LOCAL_SRC_FILES  := $(SRC_DIR)/testAirJoyServer.cpp

LOCAL_C_INCLUDES := $(INC_ALL)
LOCAL_CFLAGS     := -fexceptions         \
	                -frtti               \
	                -D__GLIBC__          \
                    -D__ANDROID__        \
                    -D__APPLE_BONJOUR__

LOCAL_SHARED_LIBRARIES := airjoy
LOCAL_LDLIBS := -llog

APP_CPPFLAGS     += -fexceptions
APP_CPPFLAGS     += -frtti

include $(BUILD_EXECUTABLE)

#--------------------------------------------------------------------
# testAirJoyClient
#--------------------------------------------------------------------
include $(CLEAR_VARS)

SRC_DIR          := ../../src/AirJoyAll/AirJoyX/test
LOCAL_MODULE     := testAirJoyClient
LOCAL_SRC_FILES  := $(SRC_DIR)/testAirJoyClient.cpp

LOCAL_C_INCLUDES := $(INC_ALL)
LOCAL_CFLAGS     := -fexceptions         \
	                -frtti               \
	                -D__GLIBC__          \
                    -D__ANDROID__        \
                    -D__APPLE_BONJOUR__

LOCAL_SHARED_LIBRARIES := airjoy airstream
LOCAL_LDLIBS := -llog

APP_CPPFLAGS     += -fexceptions
APP_CPPFLAGS     += -frtti

include $(BUILD_EXECUTABLE)
