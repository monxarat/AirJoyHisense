/*
 * AirJoyServer.cpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-15
 *
 *
 */

#include "AirJoyServer.hpp"
#include "AirJoyServerCore.hpp"

#include "AirJoyServerHandler.hpp"

#include "AirJoyDefault.hpp"
#include "AirQueryHandler.hpp"
#include "AirFileHandler.hpp"
#include "AirMediaHandler.hpp"

#include "AirMessageHandler.hpp"

#include "UmpServer.hpp"


using namespace airjoy;


AirJoyServer::AirJoyServer()
{
    m_core = new AirJoyServerCore;
}

AirJoyServer::~AirJoyServer()
{
    delete m_core;
}

bool AirJoyServer::start(AirJoyServiceInfo &info)
{   
    if (! m_core->start())
        return false;

    // load AirQuery
    typedef boost::shared_ptr<AirQueryHandler> AirQueryHandlerPointer;
    AirQueryHandlerPointer handler = boost::make_shared<AirQueryHandler>(info);
    m_core->handler()->addAirMessageHandler(AIRJOY_TOPIC_AIRQUERY, handler);

    return true;
}

bool AirJoyServer::stop(void)
{
    return m_core->stop();
}

uint16_t AirJoyServer::port(void)
{
    return m_core->server()->port();
}

bool AirJoyServer::loadAirFile(AirFileListener *listener)
{
    if (! m_core->isStart())
        return false;

    typedef boost::shared_ptr<AirFileHandler> AirFileHandlerPointer;
    AirFileHandlerPointer handler = boost::make_shared<AirFileHandler>(listener);
    m_core->handler()->addAirMessageHandler(AIRJOY_TOPIC_AIRFILE, handler);

    return true;
}

bool AirJoyServer::unloadAirFile(void)
{
    if (! m_core->isStart())
        return false;

    m_core->handler()->removeAirMessageHandler(AIRJOY_TOPIC_AIRFILE);
    
    return true;
}

bool AirJoyServer::loadAirMedia(AirMediaListener *listener)
{
    if (! m_core->isStart())
        return false;

    typedef boost::shared_ptr<AirMediaHandler> AirMediaHandlerPointer;
    AirMediaHandlerPointer handler = boost::make_shared<AirMediaHandler>(listener);
    m_core->handler()->addAirMessageHandler(AIRJOY_TOPIC_AIRMEDIA, handler);

    return true;
}

bool AirJoyServer::unloadAirMedia(void)
{
    if (! m_core->isStart())
        return false;

    m_core->handler()->removeAirMessageHandler(AIRJOY_TOPIC_AIRMEDIA);
    
    return true;
}

bool AirJoyServer::publishMediaEvent(const std::string &mediaType, 
                                     const std::string &action, 
                                     const std::string &id,
                                     const std::string &url,
                                     const std::string &name,
                                     const std::string &fromIp)
{
    if (! m_core->isStart())
        return false;
    
    m_core->handler()->publishMediaEvent(mediaType, action, id, url, name, fromIp);

    return true;
}