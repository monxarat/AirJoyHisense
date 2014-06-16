
ADD_DEFINITIONS(-DAIRJOY_STATIC)

ADD_EXECUTABLE(testTcpClient test.cpp MyTcpClient.hpp)
TARGET_LINK_LIBRARIES(testTcpClient AirUmp_static AirCommon_static AirUtil_static ${OS_LIB})
