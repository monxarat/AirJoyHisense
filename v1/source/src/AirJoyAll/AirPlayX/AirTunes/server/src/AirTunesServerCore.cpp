/*
* AirTunesServerCore.cpp
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

#include "AirTunesServerCore.hpp"
#include "AirTunesServerDefault.hpp"
#include "UmpServer.hpp"

using namespace airjoy;


AirTunesServerCore::AirTunesServerCore()
    : m_isStart(false)
    , m_timer(m_ios)
{
    // AirTunes Server
    m_pUmpServer = boost::make_shared<UmpServer>(boost::ref(m_ios), AIRTUNES_SERVER_PORT);

#if 0
	//
	// 测试用（异步调用，需要启动定时器，用来检测m_ios线程是否一直运行）
	//
	
	m_timer.expires_from_now(boost::posix_time::seconds(1));
	m_timer.async_wait(boost::bind(&AirTunesServerCore::didTimeout, 
		             this, 
		             boost::asio::placeholders::error));
#endif
}

AirTunesServerCore::~AirTunesServerCore()
{
    stop();
}

bool AirTunesServerCore::start(void)
{
    if (m_isStart)
        return false;

    if (m_pThread.use_count() > 0)
        return false;

    try
    {
        //std::cout << "AirTunesServerCore::start: " << std::endl;

        m_pThread = boost::make_shared<boost::thread>(boost::bind(&boost::asio::io_service::run, &m_ios));
    }
    catch (std::exception &e)
    {
        std::cout << "AirTunesServerCore::start: " << e.what() << std::endl;
        return false;
    }

    m_isStart = true;

    return true;
}

bool AirTunesServerCore::stop(void)
{
    //std::cout << "AirTunesServerCore::stop" << std::endl;
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

bool AirTunesServerCore::run(void)
{
    start();

    m_pThread->join();
    m_pThread.reset();
    m_ios.reset();

    return true;
}

void AirTunesServerCore::didTimeout(const boost::system::error_code& error)
{
	std::cout << "AirTunesServerCore::didTimeout" << std::endl;

	if (error)
    {
		std::cout << "AirTunesServerCore::didTimeout error : " << error << std::endl;

		return;	
	}

	//
	// 异步调用，需要启动定时器，用来保证m_ios线程一直运行
	//
	
	m_timer.expires_from_now(boost::posix_time::seconds(1));
	m_timer.async_wait(boost::bind(&AirTunesServerCore::didTimeout, 
		             this, 
		             boost::asio::placeholders::error));
}