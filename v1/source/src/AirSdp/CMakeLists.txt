#-------------------------------------------------------------------------------------
#
# module:AirSdp
#
#-------------------------------------------------------------------------------------

SET(SDP_SRC    include/Sdp.hpp
               include/SdpDiscoveryHandler.hpp
               include/SdpPublishHandler.hpp
               include/SdpServiceInfo.hpp
               include/SdpMaster.hpp
               include/SdpMasterHandler.hpp
               include/SdpWorker.hpp
               include/SdpDiscoveryWorker.hpp
               include/SdpPublishWorker.hpp
               include/SdpNetwork.hpp
               src/SdpDiscoveryWorker.cpp
               src/SdpPublishWorker.cpp
               src/SdpNetwork.cpp)

ADD_LIBRARY(AirSdp_static STATIC ${SDP_SRC})

#ADD_SUBDIRECTORY(test)

