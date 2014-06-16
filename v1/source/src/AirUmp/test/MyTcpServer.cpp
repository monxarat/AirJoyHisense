/*
 * MyTcpServer.hpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-8
 *
 */


#include "MyTcpServer.hpp"
#include "TcpServer.hpp"

using namespace airjoy;

MyTcpServer::MyTcpServer()
{

}

MyTcpServer::~MyTcpServer()
{
}

void MyTcpServer::start(void)
{
    try
    {
        boost::asio::io_service ios;

        //TcpServer::QueueType messageQ;

        //TcpServer server(boost::ref(ios), 65432, boost::ref(messageQ));
    }
    catch (std::exception &e)
    {
        std::string msg(e.what());
        LOGI("TcpServer::TcpServer() start failed");
        LOGI(msg.c_str());
    }

}
        
void MyTcpServer::stop(void)
{
}
