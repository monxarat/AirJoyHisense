/*
* AirPlayClientCore.cpp
*
*
* coding.tom@gmail.com
*
* 2012-5-23
*
* @remark
*      set tabstop=4
*      set shiftwidth=4
*      set expandtab
*/

#include "AirPlayClientCore.hpp"
#include "AnySS.hpp"
#include "UmpServer.hpp"
#include "UmpClient.hpp"
#include "Sdp.hpp"
#include "AirPlayDefault.hpp"

#ifdef __APPLE_BONJOUR__
#include "Nsd.hpp"
#endif

using namespace airjoy;

AirPlayClientCore::AirPlayClientCore()
    : m_isStart(false)
{
    // Ump Server
    m_pUmpServer = boost::make_shared<UmpServer>(boost::ref(m_ios), ANYPLAY_SERVER_PORT);

    // Ump Client
    m_pUmpClient = boost::make_shared<UmpClient>(boost::ref(m_ios));

    // Net Service Discovery
    m_pSdp = boost::make_shared<Sdp>(boost::ref(m_ios), ANYPLAY_NSD_PORT);

    // Stream Server
    m_pSS = boost::make_shared<AnySS>();

    // AirPlay Server
    m_pAirPlayServer = boost::make_shared<UmpServer>(boost::ref(m_ios), AIRPLAY_SERVER_PORT);

    // AirPlay Client
    m_pAirPlayClient = boost::make_shared<UmpClient>(boost::ref(m_ios));

#ifdef __APPLE_BONJOUR__
    // AirPlay Nsd
    m_pAirPlayNsd = boost::make_shared<Nsd>();
#endif
}

AirPlayClientCore::~AirPlayClientCore()
{
    stop();
}

bool AirPlayClientCore::start(void)
{
    if (m_isStart)
        return false;

    if (m_pThread.use_count() > 0)
        return false;

    try
    {
        m_pSdp->start();
        
        //std::cout << "Stream Service Port: " << m_pSS->port() << std::endl;

        m_pThread = boost::make_shared<boost::thread>(boost::bind(&boost::asio::io_service::run, &m_ios));
    }
    catch (std::exception &e)
    {
        //std::cout << "AirPlayClientCore::start: " << e.what() << std::endl;
        return false;
    }

    m_isStart = true;

    return true;
}

bool AirPlayClientCore::stop(void)
{
    //std::cout << "AirPlayClientCore::stop" << std::endl;
    if (! m_isStart)
        return false;

    if (m_pThread.use_count() == 0)
        return false;

    m_pSdp->stop();
    m_pUmpClient->stop();
    m_pUmpServer->stop();
    m_pAirPlayServer->stop();
    m_pAirPlayClient->stop();

    m_ios.stop();

    m_pThread->join();
    m_pThread.reset();

    m_ios.reset();

    m_isStart = false;

    return true;
}

bool AirPlayClientCore::run(void)
{
    start();

    m_pThread->join();
    m_pThread.reset();
    m_ios.reset();

    return true;
}
