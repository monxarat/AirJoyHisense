
ADD_DEFINITIONS(-DAIRJOY_STATIC)

ADD_EXECUTABLE(testUmpServer testUmpServer.cpp TestUmpServer.hpp)
TARGET_LINK_LIBRARIES(testUmpServer AirUmp_static AirCommon_static AirUtil_static ${OS_LIB})

ADD_EXECUTABLE(testMyTcpServer testMyTcpServer.cpp MyTcpServer.cpp MyTcpServer.hpp)
TARGET_LINK_LIBRARIES(testMyTcpServer AirUmp_static AirCommon_static ${OS_LIB})
