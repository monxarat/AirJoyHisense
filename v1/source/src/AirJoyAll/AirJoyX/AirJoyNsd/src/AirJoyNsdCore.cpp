/*
* AirJoyNsdCore.cpp
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

#include "AirJoyNsdCore.hpp"
#include "Sdp.hpp"
#include "AirJoyDefault.hpp"

using namespace airjoy;


AirJoyNsdCore::AirJoyNsdCore()
    : m_isStart(false)
{
    m_pSdp = boost::make_shared<Sdp>(boost::ref(m_ios), AIRJOY_NSD_PORT);

    m_pSdp->setServiceTimeout(AIRJOY_SERVICE_DISCOVERY_TIMEOUT);
}

AirJoyNsdCore::~AirJoyNsdCore()
{
    stop();
}

bool AirJoyNsdCore::start(void)
{
    if (m_isStart)
        return false;

    if (m_pThread.use_count() > 0)
        return false;

    try
    {
		m_pSdp->start();
        m_pThread = boost::make_shared<boost::thread>(boost::bind(&boost::asio::io_service::run, &m_ios));
    }
    catch (std::exception &e)
    {
        std::cout << "AirPlayNsdCore::start: " << e.what() << std::endl;
        return false;
    }

    m_isStart = true;

    return true;
}

bool AirJoyNsdCore::stop(void)
{
    //std::cout << "AirPlayNsdCore::stop" << std::endl;
    if (! m_isStart)
        return false;

    if (m_pThread.use_count() == 0)
        return false;

    m_pSdp->stop();

    m_ios.stop();

    m_pThread->join();
    m_pThread.reset();

    m_ios.reset();

    m_isStart = false;

    return true;
}

bool AirJoyNsdCore::run(void)
{
    start();

    m_pThread->join();
    m_pThread.reset();
    m_ios.reset();

    return true;
}