/*
 * AirJoyServerCore.cpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-15
 *
 *
 */

#include "AirJoyServerCore.hpp"
#include "UmpServer.hpp"
#include "AirJoyDefault.hpp"
#include "AirJoyServerHandler.hpp"

using namespace airjoy;


AirJoyServerCore::AirJoyServerCore()
    : m_isStart(false)
	, m_timer(m_ios)
{
    m_pServer = boost::make_shared<UmpServer>(boost::ref(m_ios), AIRJOY_SERVER_PORT);

    m_pServerHandler = boost::make_shared<AirJoyServerHandler>();
    m_pServer->setHandler(m_pServerHandler);

#if 0
	//
	// 异步调用，需要启动定时器，用来保证m_ios线程一直运行
	//

	m_timer.expires_from_now(boost::posix_time::seconds(1));
	m_timer.async_wait(boost::bind(&AirJoyServerCore::didTimeout,
		             this,
		             boost::asio::placeholders::error));
#endif
}

AirJoyServerCore::~AirJoyServerCore()
{
    stop();
}

bool AirJoyServerCore::start(void)
{
    if (m_isStart)
        return false;

    if (m_pThread.use_count() > 0)
        return false;

    try
    {
        m_pServer->start();
        m_pThread = boost::make_shared<boost::thread>(boost::bind(&boost::asio::io_service::run, &m_ios));
    }
    catch (std::exception &e)
    {
        return false;
    }

    m_isStart = true;

    return true;
}

bool AirJoyServerCore::stop(void)
{
    if (! m_isStart)
        return false;

    if (m_pThread.use_count() == 0)
        return false;

    m_pServer->stop();

    m_ios.stop();

    m_pThread->join();
    m_pThread.reset();

    m_ios.reset();

    m_isStart = false;

    return true;
}

bool AirJoyServerCore::run(void)
{
    if (! this->start())
        return false;

    m_pThread->join();
    m_pThread.reset();
    m_ios.reset();

    return true;
}

bool AirJoyServerCore::isStart(void)
{
    return m_isStart;
}

AirJoyServerCore::UmpServerPointer AirJoyServerCore::server(void)            
{ 
    return m_pServer; 
}

AirJoyServerCore::AirJoyServerHandlerPointer AirJoyServerCore::handler(void)
{
    return m_pServerHandler;
}

void AirJoyServerCore::didTimeout(const boost::system::error_code& error)
{
	std::cout << "AirJoyServerCore::didTimeout" << std::endl;

	if (error)
    {
		std::cout << "AirJoyServerCore::didTimeout error : " << error << std::endl;

		return;	
	}

	//
	// 异步调用，需要启动定时器，用来保证m_ios线程一直运行
	//
	
	m_timer.expires_from_now(boost::posix_time::seconds(1));
	m_timer.async_wait(boost::bind(&AirJoyServerCore::didTimeout, 
		             this, 
		             boost::asio::placeholders::error));
}