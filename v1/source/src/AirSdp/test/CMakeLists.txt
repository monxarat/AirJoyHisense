ADD_EXECUTABLE(testSdp testSdp.cpp AirSdpServer.hpp)
TARGET_LINK_LIBRARIES(testSdp AirSdp_static AirCommon_static ${OS_LIB})
