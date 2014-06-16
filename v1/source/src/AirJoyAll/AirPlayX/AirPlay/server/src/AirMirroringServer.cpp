/*
 * AirMirroringServer.cpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-15
 *
 *
 */

#include "AirMirroringServer.hpp"
#include "AirMirroringServerCore.hpp"
#include "AirMirroringServerHandler.hpp"
#include "UmpServer.hpp"

namespace airjoy
{
    typedef boost::shared_ptr<AirMirroringServerHandler>         AirMirroringServerHandlerPointer;
    AirMirroringServerHandlerPointer gAirMirroringServerHandler;
}

using namespace airjoy;

AirMirroringServer::AirMirroringServer()
    : m_isStart(false)
{
    m_core = new AirMirroringServerCore;
}

AirMirroringServer::~AirMirroringServer()
{
    delete m_core;
}

void AirMirroringServer::start(AirMirroringServerListener *listener)
{
    if (m_isStart)
        return;
    
    m_isStart = true;

    gAirMirroringServerHandler = boost::make_shared<AirMirroringServerHandler>(this, listener);

    // set service handler
    m_core->server()->setHandler(gAirMirroringServerHandler);
    m_core->server()->start();

    m_core->start();
}

void AirMirroringServer::stop(void)
{
    if (! m_isStart)
        return;
 
    m_isStart = false;

    m_core->server()->stop();
    m_core->stop();
}