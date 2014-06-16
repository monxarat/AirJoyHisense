/*
 * AirTunesServer.cpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-15
 *
 *
 */

#include "AirTunesServer.hpp"
#include "AirTunesServerCore.hpp"
#include "AirTunesServerHandler.hpp"
#include "UmpServer.hpp"
#include "DacpClient.hpp"
#include "HttpMessage.hpp"
#include "DacpClientListener.hpp"

namespace airjoy
{

    class MyDacpClientListener : public DacpClientListener
    {
    public:
        MyDacpClientListener() {}
        virtual ~MyDacpClientListener() {}

        virtual void didConnectOk(void)
        {
            //std::cout << "DacpClientListener: didConnectOk" << std::endl;
        }

        virtual void didDisConnect(void)
        {
            //std::cout << "DacpClientListener: didDisConnect" << std::endl;
        }

        virtual void didTimeout(void)
        {
            //std::cout << "DacpClientListener: didTimeout" << std::endl;
        }

        virtual void didErrorNet(void)
        {
            //std::cout << "DacpClientListener: didErrorNet" << std::endl;
        }

        virtual void didConnectFailed(void)
        {
            //std::cout << "DacpClientListener: didConnectFailed" << std::endl;
        }
    };

    MyDacpClientListener gMyDacpClientListener;

    typedef boost::shared_ptr<AirTunesServerHandler>         AirTunesServerHandlerPointer;
    AirTunesServerHandlerPointer gAirTunesServerHandler;
}

using namespace airjoy;

AirTunesServer::AirTunesServer()
    : m_isStart(false)
{
    m_core = new AirTunesServerCore;
    m_dacpClient = new DacpClient;
}

AirTunesServer::~AirTunesServer()
{
    delete m_core;
    delete m_dacpClient;
}

void AirTunesServer::start(AirTunesServiceInfo &serviceInfo, AirTunesServerListener *listener)
{
    if (m_isStart)
        return;
    
    m_isStart = true;

    gAirTunesServerHandler = boost::make_shared<AirTunesServerHandler>(this, listener);

    // set service info
    AirTunesServerHandler::AirTunesServiceInfoPointer service;
    service = boost::make_shared<AirTunesServiceInfo>(serviceInfo);
    gAirTunesServerHandler->setServiceInfo(service);

    m_core->server()->setHandler(gAirTunesServerHandler);
    m_core->server()->start();

    serviceInfo.setPort(m_core->server()->port());

    m_core->start();

    m_dacpClient->setListener(&gMyDacpClientListener);
    m_dacpClient->start();
}

void AirTunesServer::stop(void)
{
    if (! m_isStart)
        return;
    
    m_isStart = false;

    m_core->server()->stop();
    m_core->stop();

    m_dacpClient->stop();
}

uint16_t AirTunesServer::port(void)
{
    return m_core->server()->port();
}

void AirTunesServer::setDacpInfo(const std::string &ip, uint16_t port)
{
    m_dacpClient->addServer(ip, port);
}

void AirTunesServer::cleanDacpInfo(const std::string &ip)
{
    m_dacpClient->removeServer(ip);
}

void AirTunesServer::sendDacpCommand(const std::string &ip, DacpCommand cmd)
{
    if (ip.empty())
        return;
    
    //std::cout << "AirTunesServer::sendDacpCommand: " << ip << std::endl;

    HttpMessage httpMessage;
    std::string uri;

    switch (cmd)
    {
    case DACP_CMD_NEXTITEM:
        uri.append("/ctrl-int/1/nextitem");
        break;

    case DACP_CMD_PREVITEM:
        uri.append("/ctrl-int/1/previtem");
        break;

    case DACP_CMD_PAUSE:
        uri.append("/ctrl-int/1/pause");
        break;

    case DACP_CMD_PLAYPAUSE:
        uri.append("/ctrl-int/1/playpause");
        break;

    case DACP_CMD_PLAY:
        uri.append("/ctrl-int/1/play");
        break;

    case DACP_CMD_STOP:
        uri.append("/ctrl-int/1/stop");
        gAirTunesServerHandler->stopAudioPlayer(ip);
        break;
    
    case DACP_CMD_VOLUMEDOWN:
        uri.append("/ctrl-int/1/volumedown");
        break;
        
    case DACP_CMD_VOLUMEUP:
        uri.append("/ctrl-int/1/volumeup");
        break;

    default:
        return;
    }

    httpMessage.setHttpType(HttpRequest);
    httpMessage.setHttpMethod("GET");
    httpMessage.setUri(uri);
    httpMessage.setHttpVersionMajor(1);
    httpMessage.setHttpVersionMinor(1);
    httpMessage.addHeaders("Host", gAirTunesServerHandler->dacpServerHostName());
    httpMessage.addHeaders("Active-Remote", gAirTunesServerHandler->dacpServerActiveRemote());

    m_dacpClient->send(ip, httpMessage.toString());
}
