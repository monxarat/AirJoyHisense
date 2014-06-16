ADD_EXECUTABLE(testAirMirroringServer testAirMirroringServer.cpp)
TARGET_LINK_LIBRARIES(testAirMirroringServer AirPlay_shared ${OS_LIB})

