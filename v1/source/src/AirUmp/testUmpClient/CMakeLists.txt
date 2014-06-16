ADD_DEFINITIONS(-DAIRJOY_STATIC)

ADD_EXECUTABLE(testUmpClient test.cpp MyUmpServer.hpp)
TARGET_LINK_LIBRARIES(testUmpClient AirUmp_static AirCommon_static AirUtil_static ${OS_LIB})
