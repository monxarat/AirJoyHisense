#-------------------------------------------------------------------------------------
#
# module:AirUmp
#
#-------------------------------------------------------------------------------------

SET(UMP_SRC    include/JobQueue.hpp
               include/Worker.hpp
               include/NetBuffer.hpp
               include/TcpQueryMessage.hpp
               include/TcpClientMessage.hpp
               include/TcpClient.hpp
               include/TcpServerMessage.hpp
			   include/TcpServer.hpp
               include/TcpSession.hpp
               include/UmpClient.hpp
               include/UmpServer.hpp
               include/UmpServerHandler.hpp
               include/UmpClientHandler.hpp
               include/Udp.hpp
               include/UdpMessage.hpp
               include/Ump.hpp
               include/UmpHandler.hpp
			   src/TcpClient.cpp
               src/TcpSession.cpp
               src/UmpClient.cpp
               src/UmpServer.cpp
               src/Ump.cpp)

ADD_LIBRARY(AirUmp_static STATIC ${UMP_SRC})

#-------------------------------------------------------------------------------------
# test
#-------------------------------------------------------------------------------------
#ADD_SUBDIRECTORY(testUmpClient)
#ADD_SUBDIRECTORY(testTcpClient)
ADD_SUBDIRECTORY(testUmp)
