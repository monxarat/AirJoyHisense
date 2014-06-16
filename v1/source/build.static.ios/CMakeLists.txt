CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(AirJoy CXX)

#---------------------------------------------------------------------------------------
# for ios/simulate
#---------------------------------------------------------------------------------------
#SET(CMAKE_CXX_COMPILER /Developer/Platforms/iPhoneSimulator.platform/Developer/usr/bin/gcc)
#ADD_DEFINITIONS(-arch i386)
#ADD_DEFINITIONS(-isysroot /Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator4.3.sdk)

#---------------------------------------------------------------------------------------
# for ios/armv7 (xcode 4.3 - SDK: IOS4.3 -> IOS5.0)
#---------------------------------------------------------------------------------------
#SET(CMAKE_CXX_COMPILER /Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/gcc)
#ADD_DEFINITIONS(-arch armv7 -mthumb)
#ADD_DEFINITIONS(-isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS5.0.sdk)

#---------------------------------------------------------------------------------------
# for ios/armv7 (xcode 4.3.2 - SDK: IOS5.1)
#---------------------------------------------------------------------------------------
SET(CMAKE_CXX_COMPILER /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/gcc)
ADD_DEFINITIONS(-arch armv7 -mthumb)
#ADD_DEFINITIONS(-isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS5.1.sdk)
ADD_DEFINITIONS(-isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS6.1.sdk)
ADD_DEFINITIONS(-D__MAC_OSX__)
ADD_DEFINITIONS(-D__IOS__)

#---------------------------------------------------------------------------------------
#
# cxx flags
#
#---------------------------------------------------------------------------------------
SET(ROOT_DIR "${PROJECT_SOURCE_DIR}/..")

#---------------------------------------------------------------------------------------
#
# include dir
#
#---------------------------------------------------------------------------------------
SET(BOOST_DIR ${ROOT_DIR}/../../boost_1_49_0)
INCLUDE_DIRECTORIES(${BOOST_DIR})
INCLUDE_DIRECTORIES(${ROOT_DIR}/src/3rdpartInc)
INCLUDE_DIRECTORIES(${ROOT_DIR}/src/AirCommon/include)
INCLUDE_DIRECTORIES(${ROOT_DIR}/src/AirUtil/include)
INCLUDE_DIRECTORIES(${ROOT_DIR}/src/AirSdp/include)
INCLUDE_DIRECTORIES(${ROOT_DIR}/src/AirUmp/include)
INCLUDE_DIRECTORIES(${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoy/server/include)
INCLUDE_DIRECTORIES(${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoy/client/include)
INCLUDE_DIRECTORIES(${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoyNsd/include)
INCLUDE_DIRECTORIES(${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirStreamX/AirFileServer/include)

#-------------------------------------------------------------------------------------
# set output dir
#-------------------------------------------------------------------------------------
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/output)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/output)
MESSAGE(STATUS "executable output:  ${EXECUTABLE_OUTPUT_PATH}")
MESSAGE(STATUS "library output: ${LIBRARY_OUTPUT_PATH}")

#-------------------------------------------------------------------------------------
#
# libairjoy.a 
#
#-------------------------------------------------------------------------------------
SET(SRC_AirCommon           ${ROOT_DIR}/src/AirCommon/src/BoostSystemPrebuild.cpp
                            ${ROOT_DIR}/src/AirCommon/src/BoostFileSystemPrebuild.cpp
                            ${ROOT_DIR}/src/AirCommon/src/BoostDateTimePrebuild.cpp
                            ${ROOT_DIR}/src/AirCommon/src/BoostThreadPrebuild.cpp
                            ${ROOT_DIR}/src/AirCommon/src/UmpMessage.cpp
                            ${ROOT_DIR}/src/AirCommon/src/HttpMessage.cpp
                            ${ROOT_DIR}/src/AirCommon/src/HttpMessageType.cpp
                            ${ROOT_DIR}/src/AirCommon/src/HttpTextParam.cpp)

SET(SRC_AirUtil             ${ROOT_DIR}/src/AirUtil/src/UtilBase64.cpp
                            ${ROOT_DIR}/src/AirUtil/src/UtilUtf8.cpp
                            ${ROOT_DIR}/src/AirUtil/src/UtilItoa.cpp
                            ${ROOT_DIR}/src/AirUtil/src/UtilMd5.cpp
                            ${ROOT_DIR}/src/AirUtil/src/UtilType.cpp
                            ${ROOT_DIR}/src/AirUtil/src/UtilString.cpp)
                       
SET(SRC_AirSdp              ${ROOT_DIR}/src/AirSdp/src/SdpDiscoveryWorker.cpp
                            ${ROOT_DIR}/src/AirSdp/src/SdpPublishWorker.cpp
                            ${ROOT_DIR}/src/AirSdp/src/SdpNetwork.cpp)

SET(SRC_AirUmp              ${ROOT_DIR}/src/AirUmp/src/TcpClient.cpp
                            ${ROOT_DIR}/src/AirUmp/src/TcpSession.cpp
                            ${ROOT_DIR}/src/AirUmp/src/UmpClient.cpp
                            ${ROOT_DIR}/src/AirUmp/src/UmpServer.cpp
                            ${ROOT_DIR}/src/AirUmp/src/Ump.cpp)

SET(SRC_AirJoy_Nsd          ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoyNsd/include/AirJoyNsd.hpp
                            ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoyNsd/include/AirJoyNsdCore.hpp
                            ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoyNsd/include/AirJoyNsdDiscoveryHandler.hpp
                            ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoyNsd/include/AirJoyNsdDiscoveryListener.hpp
                            ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoyNsd/include/AirJoyNsdPublishHandler.hpp
                            ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoyNsd/include/AirJoyNsdPublishListener.hpp
                            ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoyNsd/src/AirJoyNsd.cpp
                            ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoyNsd/src/AirJoyNsdCore.cpp
                            ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoyNsd/src/AirJoyNsdDiscoveryHandler.cpp
                            ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoyNsd/src/AirJoyNsdPublishHandler.cpp)

SET(SRC_AirJoy_Server       ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoy/server/src/AirJoyDefault.cpp
                            ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoy/server/src/AirJoyServer.cpp
                            ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoy/server/src/AirJoyServerCore.cpp
                            ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoy/server/src/AirJoyServerHandler.cpp
                            ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoy/server/src/AirQueryHandler.cpp
                            ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoy/server/src/AirFileHandler.cpp
                            ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoy/server/src/AirMediaHandler.cpp
                            ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoy/server/src/AirMediaEvent.cpp)

SET(SRC_AirJoy_Client       ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoy/client/src/AirJoyClient.cpp
                            ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoy/client/src/AirJoyClientCore.cpp
                            ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoy/client/src/AirMediaClient.cpp
                            ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoy/client/src/AirMediaClientQueryHandler.cpp
                            ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoy/client/src/AirMediaClientPhoto.cpp
                            ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoy/client/src/AirMediaClientVideo.cpp
                            ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoy/client/src/AirMediaClientAudio.cpp
                            ${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoy/client/src/AirMediaClientVolume.cpp)

SET(SRC_AirStreamX          ${ROOT_DIR}/src/AirJoyAll/AirStreamX/AirFileServer/src/AirFileServer.cpp
                            ${ROOT_DIR}/src/AirJoyAll/AirStreamX/AirFileServer/src/AirHttpServer.cpp
                            ${ROOT_DIR}/src/AirJoyAll/AirStreamX/AirFileServer/src/mongoose.cpp)

ADD_LIBRARY(airjoy_static   ${SRC_AirCommon}
                            ${SRC_AirUtil}
                            ${SRC_AirSdp}
                            ${SRC_AirUmp}
                            ${SRC_AirJoy_Nsd}
                            ${SRC_AirJoy_Server}
                            ${SRC_AirJoy_Client})

SET_TARGET_PROPERTIES(airjoy_static PROPERTIES OUTPUT_NAME "airjoy")
