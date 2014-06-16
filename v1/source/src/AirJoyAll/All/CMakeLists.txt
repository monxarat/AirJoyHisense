#-------------------------------------------------------------------------------------
#
# module:AirJoyX
#
#-------------------------------------------------------------------------------------

#-------------------------------------------------------------------------------------
# AirStream
#-------------------------------------------------------------------------------------
SET(AirFileServer_SRC   ../AirStreamX/AirFileServer/include/AirFileServer.hpp
                        ../AirStreamX/AirFileServer/include/AirHttpServer.hpp
                        ../AirStreamX/AirFileServer/include/mongoose.hpp
                        ../AirStreamX/AirFileServer/src/AirFileServer.cpp
                        ../AirStreamX/AirFileServer/src/AirHttpServer.cpp
                        ../AirStreamX/AirFileServer/src/mongoose.cpp)

#-------------------------------------------------------------------------------------
# AirJoy
#-------------------------------------------------------------------------------------
SET(AirJoyNsd_SRC   ../AirJoyX/AirJoyNsd/include/AirJoyNsd.hpp
                    ../AirJoyX/AirJoyNsd/include/AirJoyNsdCore.hpp
                    ../AirJoyX/AirJoyNsd/include/AirJoyNsdDiscoveryHandler.hpp
                    ../AirJoyX/AirJoyNsd/include/AirJoyNsdDiscoveryListener.hpp
                    ../AirJoyX/AirJoyNsd/include/AirJoyNsdPublishHandler.hpp
                    ../AirJoyX/AirJoyNsd/include/AirJoyNsdPublishListener.hpp
                    ../AirJoyX/AirJoyNsd/src/AirJoyNsd.cpp
                    ../AirJoyX/AirJoyNsd/src/AirJoyNsdCore.cpp
                    ../AirJoyX/AirJoyNsd/src/AirJoyNsdDiscoveryHandler.cpp
                    ../AirJoyX/AirJoyNsd/src/AirJoyNsdPublishHandler.cpp)

SET(AirJoyServer_SRC  ../AirJoyX/AirJoy/server/include/AirJoyServiceInfo.hpp
                      ../AirJoyX/AirJoy/server/include/AirJoyDefault.hpp
                      ../AirJoyX/AirJoy/server/include/AirJoyServer.hpp
                      ../AirJoyX/AirJoy/server/include/AirJoyServerCore.hpp
                      ../AirJoyX/AirJoy/server/include/AirJoyServerHandler.hpp
                      ../AirJoyX/AirJoy/server/include/AirMessageHandler.hpp
                      ../AirJoyX/AirJoy/server/include/AirQueryHandler.hpp
                      ../AirJoyX/AirJoy/server/include/AirFileHandler.hpp
                      ../AirJoyX/AirJoy/server/include/AirFileListener.hpp
                      ../AirJoyX/AirJoy/server/include/AirMediaHandler.hpp
                      ../AirJoyX/AirJoy/server/include/AirMediaListener.hpp
                      ../AirJoyX/AirJoy/server/include/AirMediaEvent.hpp
                      ../AirJoyX/AirJoy/server/include/ParamPhoto.hpp
                      ../AirJoyX/AirJoy/server/include/ParamVideo.hpp
                      ../AirJoyX/AirJoy/server/include/ParamAudio.hpp
                      ../AirJoyX/AirJoy/server/include/ParamVolume.hpp
                      ../AirJoyX/AirJoy/server/src/AirJoyDefault.cpp
                      ../AirJoyX/AirJoy/server/src/AirJoyServer.cpp
                      ../AirJoyX/AirJoy/server/src/AirJoyServerCore.cpp
                      ../AirJoyX/AirJoy/server/src/AirJoyServerHandler.cpp
                      ../AirJoyX/AirJoy/server/src/AirQueryHandler.cpp
                      ../AirJoyX/AirJoy/server/src/AirFileHandler.cpp
                      ../AirJoyX/AirJoy/server/src/AirMediaHandler.cpp
                      ../AirJoyX/AirJoy/server/src/AirMediaEvent.cpp)

SET(AirJoyClient_SRC      ../AirJoyX/AirJoy/client/include/AirJoyClient.hpp
                          ../AirJoyX/AirJoy/client/include/AirJoyClientCore.hpp
                          ../AirJoyX/AirJoy/client/include/AirMediaClient.hpp
                          ../AirJoyX/AirJoy/client/include/AirMediaClientQueryHandler.hpp
                          ../AirJoyX/AirJoy/client/include/AirMediaClientQueryListener.hpp
                          ../AirJoyX/AirJoy/client/include/AirMediaClientPhoto.hpp
                          ../AirJoyX/AirJoy/client/include/AirMediaClientVideo.hpp
                          ../AirJoyX/AirJoy/client/include/AirMediaClientAudio.hpp
                          ../AirJoyX/AirJoy/client/include/AirMediaClientVolume.hpp
                          ../AirJoyX/AirJoy/client/include/ResultPhoto.hpp
                          ../AirJoyX/AirJoy/client/include/ResultVideo.hpp
                          ../AirJoyX/AirJoy/client/include/ResultAudio.hpp
                          ../AirJoyX/AirJoy/client/include/ResultVolume.hpp
                          ../AirJoyX/AirJoy/client/src/AirJoyClient.cpp
                          ../AirJoyX/AirJoy/client/src/AirJoyClientCore.cpp
                          ../AirJoyX/AirJoy/client/src/AirMediaClient.cpp
                          ../AirJoyX/AirJoy/client/src/AirMediaClientQueryHandler.cpp
                          ../AirJoyX/AirJoy/client/src/AirMediaClientPhoto.cpp
                          ../AirJoyX/AirJoy/client/src/AirMediaClientVideo.cpp
                          ../AirJoyX/AirJoy/client/src/AirMediaClientAudio.cpp
                          ../AirJoyX/AirJoy/client/src/AirMediaClientVolume.cpp)

SET(AirJoy_SRC            ${AirJoyServer_SRC} 
                          ${AirJoyClient_SRC} 
                          ${AirJoyNsd_SRC})

#-------------------------------------------------------------------------------------
# AirPlay
#-------------------------------------------------------------------------------------
SET(PLIST_INC ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdpartSrc/PlistCpp/include)
INCLUDE_DIRECTORIES(${PLIST_INC})

SET(Common_SRC      ../AirPlayX/Common/include/AirPlayServiceInfo.hpp
                    ../AirPlayX/Common/include/AirTunesServiceInfo.hpp
                    ../AirPlayX/Common/src/AirPlayServiceInfo.cpp
                    ../AirPlayX/Common/src/AirTunesServiceInfo.cpp)

SET(AirPlayNsd_SRC    ../AirPlayX/AirPlayNsd/include/AirPlayNsd.hpp
                      ../AirPlayX/AirPlayNsd/include/AirPlayNsdCore.hpp
                      ../AirPlayX/AirPlayNsd/include/AirPlayNsdDiscoveryHandler.hpp
                      ../AirPlayX/AirPlayNsd/include/AirPlayNsdDiscoveryListener.hpp
                      ../AirPlayX/AirPlayNsd/include/AirPlayNsdPublishHandler.hpp
                      ../AirPlayX/AirPlayNsd/include/AirPlayNsdPublishListener.hpp
                      ../AirPlayX/AirPlayNsd/src/AirPlayNsd.cpp
                      ../AirPlayX/AirPlayNsd/src/AirPlayNsdCore.cpp
                      ../AirPlayX/AirPlayNsd/src/AirPlayNsdDiscoveryHandler.cpp
                      ../AirPlayX/AirPlayNsd/src/AirPlayNsdPublishHandler.cpp)

SET(AirPlayServer_SRC    ../AirPlayX/AirPlay/server/include/AirPlayServerDefault.hpp
                         ../AirPlayX/AirPlay/server/include/AirPlayServer.hpp
                         ../AirPlayX/AirPlay/server/include/AirPlayServerCore.hpp
                         ../AirPlayX/AirPlay/server/include/AirPlayServerHandler.hpp
                         ../AirPlayX/AirPlay/server/include/AirPlayServerListener.hpp
                         ../AirPlayX/AirPlay/server/include/AirPlayEvent.hpp
                         ../AirPlayX/AirPlay/server/src/AirPlayServer.cpp
                         ../AirPlayX/AirPlay/server/src/AirPlayServerCore.cpp
                         ../AirPlayX/AirPlay/server/src/AirPlayServerHandler.cpp
                         ../AirPlayX/AirPlay/server/src/AirPlayEvent.cpp)

SET(ALAC_SRC             ../AirPlayX/AirTunes/server/include/stdint_win.h
                         ../AirPlayX/AirTunes/server/include/alac.h
                         ../AirPlayX/AirTunes/server/src/alac.cpp)

ADD_LIBRARY(libalac_x_static STATIC ${ALAC_SRC})
SET_TARGET_PROPERTIES(libalac_x_static PROPERTIES OUTPUT_NAME "libalac_x")

IF(WIN32)
INCLUDE_DIRECTORIES(../AirPlayX/AirTunes/libao_win32)

SET(AirTunesServer_SRC   ../AirPlayX/AirTunes/server/include/AirTunesServerDefault.hpp
                         ../AirPlayX/AirTunes/server/include/AirTunesServer.hpp
                         ../AirPlayX/AirTunes/server/include/AirTunesServerCore.hpp
                         ../AirPlayX/AirTunes/server/include/AirTunesServerHandler.hpp
                         ../AirPlayX/AirTunes/server/include/AirTunesServerListener.hpp
                         ../AirPlayX/AirTunes/server/include/AirCrypto.hpp
                         ../AirPlayX/AirTunes/server/include/AirBase64.hpp
						 ../AirPlayX/AirTunes/server/include/RtspMessage.hpp
                         ../AirPlayX/AirTunes/server/include/AirPort.hpp
                         ../AirPlayX/AirTunes/server/include/AirPortControlHandler.hpp
                         ../AirPlayX/AirTunes/server/include/AirPortDataHandler.hpp
                         ../AirPlayX/AirTunes/server/include/AirPortTimingHandler.hpp
                         ../AirPlayX/AirTunes/libao_win32/ao/ao.h
                         ../AirPlayX/AirTunes/libao_win32/ao/ao_private.h
                         ../AirPlayX/AirTunes/libao_win32/ao/os_types.h
                         ../AirPlayX/AirTunes/server/include/alac.h
						 ../AirPlayX/AirTunes/server/include/dmap_parser.h
						 ../AirPlayX/AirTunes/server/include/Dmap.hpp
                         ../AirPlayX/AirTunes/server/include/AudioBuffer.hpp
                         ../AirPlayX/AirTunes/server/include/AudioDriver.hpp
                         ../AirPlayX/AirTunes/server/include/AudioPlayer.hpp
                         ../AirPlayX/AirTunes/server/include/DacpClient.hpp
                         ../AirPlayX/AirTunes/server/include/DacpClientCore.hpp
                         ../AirPlayX/AirTunes/server/include/DacpClientHandler.hpp
                         ../AirPlayX/AirTunes/server/include/DacpClientListener.hpp
                         ../AirPlayX/AirTunes/server/src/AirTunesServer.cpp
                         ../AirPlayX/AirTunes/server/src/AirTunesServerCore.cpp
                         ../AirPlayX/AirTunes/server/src/AirTunesServerHandler.cpp
                         ../AirPlayX/AirTunes/server/src/AirCrypto.cpp
                         ../AirPlayX/AirTunes/server/src/AirBase64.cpp
						 ../AirPlayX/AirTunes/server/src/RtspMessage.cpp
                         ../AirPlayX/AirTunes/server/src/AirPort.cpp
                         ../AirPlayX/AirTunes/server/src/AirPortControlHandler.cpp
                         ../AirPlayX/AirTunes/server/src/AirPortDataHandler.cpp
                         ../AirPlayX/AirTunes/server/src/AirPortTimingHandler.cpp
                         ../AirPlayX/AirTunes/server/src/dmap_parser.cpp
                         ../AirPlayX/AirTunes/server/src/Dmap.cpp
						 ../AirPlayX/AirTunes/server/src/AudioBuffer.cpp
                         ../AirPlayX/AirTunes/server/src/AudioDriver.cpp
                         ../AirPlayX/AirTunes/server/src/AudioPlayer.cpp
                         ../AirPlayX/AirTunes/server/src/DacpClient.cpp
                         ../AirPlayX/AirTunes/server/src/DacpClientCore.cpp
                         ../AirPlayX/AirTunes/server/src/DacpClientHandler.cpp)

SET(AO_SRC               ../AirPlayX/AirTunes/libao_win32/ao/ao.h
                         ../AirPlayX/AirTunes/libao_win32/ao_wmm.cpp)

ADD_LIBRARY(libao_x_static STATIC ${AO_SRC})
SET_TARGET_PROPERTIES(libao_x_static PROPERTIES OUTPUT_NAME "libao_x")

ELSE(WIN32)
INCLUDE_DIRECTORIES("/opt/local/include")

SET(AirTunesServer_SRC   ../AirPlayX/AirTunes/server/include/AirTunesServerDefault.hpp
                         ../AirPlayX/AirTunes/server/include/AirTunesServer.hpp
                         ../AirPlayX/AirTunes/server/include/AirTunesServerCore.hpp
                         ../AirPlayX/AirTunes/server/include/AirTunesServerHandler.hpp
                         ../AirPlayX/AirTunes/server/include/AirTunesServerListener.hpp
                         ../AirPlayX/AirTunes/server/include/AirCrypto.hpp
                         ../AirPlayX/AirTunes/server/include/AirBase64.hpp
						 ../AirPlayX/AirTunes/server/include/RtspMessage.hpp
                         ../AirPlayX/AirTunes/server/include/AirPort.hpp
                         ../AirPlayX/AirTunes/server/include/AirPortControlHandler.hpp
                         ../AirPlayX/AirTunes/server/include/AirPortDataHandler.hpp
                         ../AirPlayX/AirTunes/server/include/AirPortTimingHandler.hpp
                         ../AirPlayX/AirTunes/server/include/alac.h
						 ../AirPlayX/AirTunes/server/include/dmap_parser.h
						 ../AirPlayX/AirTunes/server/include/Dmap.hpp
                         ../AirPlayX/AirTunes/server/include/AudioBuffer.hpp
                         ../AirPlayX/AirTunes/server/include/AudioDriver.hpp
                         ../AirPlayX/AirTunes/server/include/AudioPlayer.hpp
                         ../AirPlayX/AirTunes/server/include/DacpClient.hpp
                         ../AirPlayX/AirTunes/server/include/DacpClientCore.hpp
                         ../AirPlayX/AirTunes/server/include/DacpClientHandler.hpp
                         ../AirPlayX/AirTunes/server/include/DacpClientListener.hpp
                         ../AirPlayX/AirTunes/server/src/AirTunesServer.cpp
                         ../AirPlayX/AirTunes/server/src/AirTunesServerCore.cpp
                         ../AirPlayX/AirTunes/server/src/AirTunesServerHandler.cpp
                         ../AirPlayX/AirTunes/server/src/AirCrypto.cpp
                         ../AirPlayX/AirTunes/server/src/AirBase64.cpp
						 ../AirPlayX/AirTunes/server/src/RtspMessage.cpp
                         ../AirPlayX/AirTunes/server/src/AirPort.cpp
                         ../AirPlayX/AirTunes/server/src/AirPortControlHandler.cpp
                         ../AirPlayX/AirTunes/server/src/AirPortDataHandler.cpp
                         ../AirPlayX/AirTunes/server/src/AirPortTimingHandler.cpp
                         ../AirPlayX/AirTunes/server/src/dmap_parser.cpp
                         ../AirPlayX/AirTunes/server/src/Dmap.cpp
						 ../AirPlayX/AirTunes/server/src/AudioBuffer.cpp
                         ../AirPlayX/AirTunes/server/src/AudioDriver.cpp
                         ../AirPlayX/AirTunes/server/src/AudioPlayer.cpp
                         ../AirPlayX/AirTunes/server/src/DacpClient.cpp
                         ../AirPlayX/AirTunes/server/src/DacpClientCore.cpp
                         ../AirPlayX/AirTunes/server/src/DacpClientHandler.cpp)
ENDIF(WIN32)

IF(WIN32)
  SET(AirPlay_SRC            ${Common_SRC} 
                             ${AirPlayNsd_SRC} 
                             ${AirPlayServer_SRC}
                             ${AirTunesServer_SRC}
                             ${AO_SRC})
ELSE(WIN32)
  IF(APPLE)
    SET(AirPlay_SRC          ${Common_SRC} 
                             ${AirPlayNsd_SRC} 
                             ${AirPlayServer_SRC}
                             ${AirTunesServer_SRC})
  ELSE(APPLE)
    SET(AirPlay_SRC          ${Common_SRC} 
                             ${AirPlayNsd_SRC} 
                             ${AirPlayServer_SRC}
                             ${AirTunesServer_SRC})
  ENDIF(APPLE)
ENDIF(WIN32)

#---------------------------------------------------------------------------------------
#
# AirJoyX include: airjoy airplay airtunes airstream
#
#---------------------------------------------------------------------------------------
ADD_LIBRARY(All_shared SHARED ${AirFileServer_SRC} ${AirJoy_SRC} ${AirPlay_SRC}) 

SET_TARGET_PROPERTIES(All_shared PROPERTIES OUTPUT_NAME "airjoyx")

IF(WIN32)
  TARGET_LINK_LIBRARIES(All_shared AirSdp_static
                                       AirUmp_static
                                       AirCommon_static
                                       AirUtil_static
                                       AirNsd_static
                                       Plist_static
                                       libalac_x_static
                                       libao_x_static
                                       ${OS_LIB}
									   ${DNSSD_LIB}
									   ${OPENSSL_LIB})
ELSE(WIN32)
  IF(APPLE)
    TARGET_LINK_LIBRARIES(All_shared AirSdp_static
                                         AirUmp_static
                                         AirCommon_static
                                         AirUtil_static
                                         AirNsd_static
                                         Plist_static
                                         libalac_x_static
                                         ${OS_LIB}
										 ${DNSSD_LIB}
										 ${OPENSSL_LIB}
                                         ${AO_LIB})
  ELSE(APPLE)
    TARGET_LINK_LIBRARIES(All_shared AirSdp_static
                                         AirUmp_static
                                         AirCommon_static
                                         AirUtil_static
                                         AirNsd_static
                                         Plist_static
                                         libalac_x_static
                                         ${OS_LIB}
										 ${DNSSD_LIB}
										 ${OPENSSL_LIB}
                                         ${AO_LIB})
  ENDIF(APPLE)
ENDIF(WIN32)

#-------------------------------------------------------------------------------------
#
# test
#
#-------------------------------------------------------------------------------------
ADD_SUBDIRECTORY(testAirJoyClient)
ADD_SUBDIRECTORY(testAirJoyServer)
ADD_SUBDIRECTORY(testAirPlayServer)
ADD_SUBDIRECTORY(testAirTunesServer)
