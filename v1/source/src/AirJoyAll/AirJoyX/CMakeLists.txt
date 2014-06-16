#-------------------------------------------------------------------------------------
#
# module:AirJoyX
#
#-------------------------------------------------------------------------------------


SET(AirJoyNsd_SRC   AirJoyNsd/include/AirJoyNsd.hpp
                    AirJoyNsd/include/AirJoyNsdCore.hpp
                    AirJoyNsd/include/AirJoyNsdDiscoveryHandler.hpp
                    AirJoyNsd/include/AirJoyNsdDiscoveryListener.hpp
                    AirJoyNsd/include/AirJoyNsdPublishHandler.hpp
                    AirJoyNsd/include/AirJoyNsdPublishListener.hpp
                    AirJoyNsd/src/AirJoyNsd.cpp
                    AirJoyNsd/src/AirJoyNsdCore.cpp
                    AirJoyNsd/src/AirJoyNsdDiscoveryHandler.cpp
                    AirJoyNsd/src/AirJoyNsdPublishHandler.cpp)

SET(AirJoyServer_SRC      AirJoy/server/include/AirJoyServiceInfo.hpp
                          AirJoy/server/include/AirJoyDefault.hpp
                          AirJoy/server/include/AirJoyServer.hpp
                          AirJoy/server/include/AirJoyServerCore.hpp
                          AirJoy/server/include/AirJoyServerHandler.hpp
                          AirJoy/server/include/AirMessageHandler.hpp
                          AirJoy/server/include/AirQueryHandler.hpp
                          AirJoy/server/include/AirFileHandler.hpp
                          AirJoy/server/include/AirFileListener.hpp
                          AirJoy/server/include/AirMediaHandler.hpp
                          AirJoy/server/include/AirMediaListener.hpp
                          AirJoy/server/include/AirMediaEvent.hpp
                          AirJoy/server/include/ParamPhoto.hpp
                          AirJoy/server/include/ParamVideo.hpp
                          AirJoy/server/include/ParamAudio.hpp
                          AirJoy/server/include/ParamVolume.hpp
                          AirJoy/server/src/AirJoyDefault.cpp
                          AirJoy/server/src/AirJoyServer.cpp
                          AirJoy/server/src/AirJoyServerCore.cpp
                          AirJoy/server/src/AirJoyServerHandler.cpp
                          AirJoy/server/src/AirQueryHandler.cpp
                          AirJoy/server/src/AirFileHandler.cpp
                          AirJoy/server/src/AirMediaHandler.cpp
                          AirJoy/server/src/AirMediaEvent.cpp)

SET(AirJoyClient_SRC      AirJoy/client/include/AirJoyClient.hpp
                          AirJoy/client/include/AirJoyClientCore.hpp
                          AirJoy/client/include/AirMediaClient.hpp
                          AirJoy/client/include/AirMediaClientQueryHandler.hpp
                          AirJoy/client/include/AirMediaClientQueryListener.hpp
                          AirJoy/client/include/AirMediaClientPhoto.hpp
                          AirJoy/client/include/AirMediaClientVideo.hpp
                          AirJoy/client/include/AirMediaClientAudio.hpp
                          AirJoy/client/include/AirMediaClientVolume.hpp
                          AirJoy/client/include/ResultPhoto.hpp
                          AirJoy/client/include/ResultVideo.hpp
                          AirJoy/client/include/ResultAudio.hpp
                          AirJoy/client/include/ResultVolume.hpp
                          AirJoy/client/src/AirJoyClient.cpp
                          AirJoy/client/src/AirJoyClientCore.cpp
                          AirJoy/client/src/AirMediaClient.cpp
                          AirJoy/client/src/AirMediaClientQueryHandler.cpp
                          AirJoy/client/src/AirMediaClientPhoto.cpp
                          AirJoy/client/src/AirMediaClientVideo.cpp
                          AirJoy/client/src/AirMediaClientAudio.cpp
                          AirJoy/client/src/AirMediaClientVolume.cpp)

#---------------------------------------------------------------------------------------
#
# AirJoy
#
#---------------------------------------------------------------------------------------
ADD_LIBRARY(AirJoy_shared SHARED ${AirJoyServer_SRC} ${AirJoyClient_SRC} ${AirJoyNsd_SRC})
SET_TARGET_PROPERTIES(AirJoy_shared PROPERTIES OUTPUT_NAME "airjoy")
TARGET_LINK_LIBRARIES(AirJoy_shared AirSdp_static
                                    AirUmp_static
                                    AirCommon_static
                                    AirUtil_static
                                    ${OS_LIB})


#-------------------------------------------------------------------------------------
#
# test
#
#-------------------------------------------------------------------------------------
ADD_SUBDIRECTORY(test)
