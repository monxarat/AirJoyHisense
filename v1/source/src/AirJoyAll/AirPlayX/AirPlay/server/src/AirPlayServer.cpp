/*
 * AirPlayServer.cpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-15
 *
 *
 */

#include "AirPlayServer.hpp"
#include "AirPlayServerCore.hpp"
#include "UmpServer.hpp"
#include "AirPlayServerHandler.hpp"

namespace airjoy
{
    typedef boost::shared_ptr<AirPlayServerHandler>         AirPlayServerHandlerPointer;
    AirPlayServerHandlerPointer gAirPlayServerHandler;
}

using namespace airjoy;

AirPlayServer::AirPlayServer()
    : m_isStart(false)
{
    m_core = new AirPlayServerCore;
}

AirPlayServer::~AirPlayServer()
{
    delete m_core;
}

void AirPlayServer::start(AirPlayServiceInfo &serviceInfo, AirPlayServerListener *listener)
{
    if (m_isStart)
        return;
    
    m_isStart = true;

    gAirPlayServerHandler = boost::make_shared<AirPlayServerHandler>(this, listener);

    // set service info
    AirPlayServerHandler::AirPlayServiceInfoPointer service;
    service = boost::make_shared<AirPlayServiceInfo>(serviceInfo);
    gAirPlayServerHandler->setServiceInfo(service);

    m_core->server()->setHandler(gAirPlayServerHandler);
    m_core->server()->start();

    serviceInfo.setPort(m_core->server()->port());

    m_core->start();
}

void AirPlayServer::stop(void)
{
    if (! m_isStart)
        return;
 
    m_isStart = false;

    m_core->server()->stop();
    m_core->stop();
}

uint16_t AirPlayServer::port(void)
{
    return m_core->server()->port();
}

void AirPlayServer::publishEvent(EventCategory type, EventAction action, const std::string &clientIp)
{
    if (gAirPlayServerHandler.use_count() == 0)
        return;

    gAirPlayServerHandler->publishEvent(type, action, clientIp);
}

void AirPlayServer::closeConnection(const std::string &clientIp)
{
    if (gAirPlayServerHandler.use_count() == 0)
        return;

    gAirPlayServerHandler->closeConnection(clientIp);
}

void AirPlayServer::getSlideshowsPicture(const std::string &clientIp)
{
    if (gAirPlayServerHandler.use_count() == 0)
        return;

    gAirPlayServerHandler->getSlideshowsPicture(clientIp);
}