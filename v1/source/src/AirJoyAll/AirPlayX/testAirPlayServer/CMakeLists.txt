ADD_EXECUTABLE(testAirPlayServer testAirPlayServer.cpp)
TARGET_LINK_LIBRARIES(testAirPlayServer AirPlay_shared ${OS_LIB})
