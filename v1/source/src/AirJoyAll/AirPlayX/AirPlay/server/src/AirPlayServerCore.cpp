/*
* AirPlayServerCore.cpp
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

#include "AirPlayServerCore.hpp"
#include "UmpServer.hpp"
#include "AirPlayServerDefault.hpp"

using namespace airjoy;


AirPlayServerCore::AirPlayServerCore()
    : m_isStart(false)
{
    // AirPlay Server
    m_pUmpServer = boost::make_shared<UmpServer>(boost::ref(m_ios), AIRPLAY_SERVER_PORT);
}

AirPlayServerCore::~AirPlayServerCore()
{
    stop();
}

bool AirPlayServerCore::start(void)
{
    if (m_isStart)
        return false;

    if (m_pThread.use_count() > 0)
        return false;

    try
    {
        m_pThread = boost::make_shared<boost::thread>(boost::bind(&boost::asio::io_service::run, &m_ios));
    }
    catch (std::exception &e)
    {
        //std::cout << "AirPlayServerCore::start: " << e.what() << std::endl;
        return false;
    }

    m_isStart = true;

    return true;
}

bool AirPlayServerCore::stop(void)
{
    //std::cout << "AirPlayServerCore::stop" << std::endl;
    if (! m_isStart)
        return false;

    if (m_pThread.use_count() == 0)
        return false;

    m_pUmpServer->stop();

    m_ios.stop();

    m_pThread->join();
    m_pThread.reset();

    m_ios.reset();

    m_isStart = false;

    return true;
}

bool AirPlayServerCore::run(void)
{
    if (! this->start())
        return false;

    m_pThread->join();
    m_pThread.reset();
    m_ios.reset();

    return true;
}

void AirPlayServerCore::send(int sessionId, const std::string &message)
{
    m_pUmpServer->send(sessionId, message);
}