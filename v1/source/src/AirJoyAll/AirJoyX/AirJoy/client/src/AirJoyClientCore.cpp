/*
 * AirJoyClientCore.cpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-15
 *
 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#include "AirJoyClientCore.hpp"
#include "UmpClient.hpp"
#include "AirJoyDefault.hpp"

using namespace airjoy;


AirJoyClientCore::AirJoyClientCore()
    : m_isStart(false)
	, m_timer(m_ios)
{
    m_pClient = boost::make_shared<UmpClient>(boost::ref(m_ios));

	//
	// 异步调用，需要启动定时器，用来保证m_ios线程一直运行
	//
	
	m_timer.expires_from_now(boost::posix_time::seconds(60));
	m_timer.async_wait(boost::bind(&AirJoyClientCore::didTimeout, 
		             this, 
		             boost::asio::placeholders::error));
}

AirJoyClientCore::~AirJoyClientCore()
{
    stop();
}

bool AirJoyClientCore::start(void)
{
    if (m_isStart)
        return false;

    if (m_pThread.use_count() > 0)
        return false;

    try
	{
		m_pClient->start();

        m_pThread = boost::make_shared<boost::thread>(boost::bind(&boost::asio::io_service::run, &m_ios));
    }
    catch (std::exception &e)
    {
        return false;
    }

    m_isStart = true;

    return true;
}

bool AirJoyClientCore::stop(void)
{
    if (! m_isStart)
        return false;

    if (m_pThread.use_count() == 0)
        return false;

    m_pClient->stop();

    m_ios.stop();

    m_pThread->join();
    m_pThread.reset();

    m_ios.reset();

    m_isStart = false;

    return true;
}

bool AirJoyClientCore::run(void)
{
    start();

    m_pThread->join();
    m_pThread.reset();
    m_ios.reset();

    return true;
}

void AirJoyClientCore::didTimeout(const boost::system::error_code& error)
{
	if (error)
    {
		return;	
	}

	//
	// 异步调用，需要启动定时器，用来保证m_ios线程一直运行
	//
	
	m_timer.expires_from_now(boost::posix_time::seconds(3600));
	m_timer.async_wait(boost::bind(&AirJoyClientCore::didTimeout, 
		             this, 
		             boost::asio::placeholders::error));
}