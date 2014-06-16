ADD_EXECUTABLE(testAirJoyServer testAirJoyServer.cpp)
TARGET_LINK_LIBRARIES(testAirJoyServer AirJoy_shared ${OS_LIB})

ADD_EXECUTABLE(testAirJoyClient testAirJoyClient.cpp)
TARGET_LINK_LIBRARIES(testAirJoyClient AirJoy_shared AirStream_shared ${OS_LIB})
