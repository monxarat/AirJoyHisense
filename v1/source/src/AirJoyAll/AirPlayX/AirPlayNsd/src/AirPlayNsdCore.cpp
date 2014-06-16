/*
* AirPlayNsdCore.cpp
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

#include "AirPlayNsdCore.hpp"
#include "Nsd.hpp"

#define UPDATE_SERVICE_INTERNAL     4

using namespace airjoy;


AirPlayNsdCore::AirPlayNsdCore()
    : m_isStart(false)
    , m_timer(m_ios)
{
    m_pNsd = boost::make_shared<Nsd>();

#if 0
    m_timer.expires_from_now(boost::posix_time::seconds(UPDATE_SERVICE_INTERNAL));
    m_timer.async_wait(boost::bind(&AirPlayNsdCore::didTimeout,
		                 this,
		                 boost::asio::placeholders::error));
#endif
}

AirPlayNsdCore::~AirPlayNsdCore()
{
    stop();
}

bool AirPlayNsdCore::start(void)
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
        //std::cout << "AirPlayNsdCore::start: " << e.what() << std::endl;
        return false;
    }

    m_isStart = true;

    return true;
}

bool AirPlayNsdCore::stop(void)
{
    //std::cout << "AirPlayNsdCore::stop" << std::endl;
    if (! m_isStart)
        return false;

    if (m_pThread.use_count() == 0)
        return false;

    m_pNsd->stopServiceDiscovery();

    m_ios.stop();

    m_pThread->join();
    m_pThread.reset();

    m_ios.reset();

    m_isStart = false;

    return true;
}

bool AirPlayNsdCore::run(void)
{
    start();

    m_pThread->join();
    m_pThread.reset();
    m_ios.reset();

    return true;
}

bool AirPlayNsdCore::addService(ServicePointer s)
{
    m_services.push_back(s); 
    return true;
}

bool AirPlayNsdCore::removeService(ServicePointer s)
{
    m_services.remove(s);
    return true;
}

bool AirPlayNsdCore::removeAllService(void)
{
    m_services.clear();
    return true;
}

void AirPlayNsdCore::didTimeout(const boost::system::error_code& error)
{
	//std::cout << "AirPlayNsdCore::didTimeout" << std::endl;

	if (error)
    {
		//std::cout << "AirPlayNsdCore::didTimeout error : " << error << std::endl;
		return;	
	}

    std::list<ServicePointer>::iterator iter;
    for (iter = m_services.begin(); iter != m_services.end(); ++iter)
    {
        m_pNsd->doForceReAnnounceService(*iter);
    }

	//
	// 异步调用，需要启动定时器，用来保证m_ios线程一直运行
	//

	m_timer.expires_from_now(boost::posix_time::seconds(UPDATE_SERVICE_INTERNAL));
	m_timer.async_wait(boost::bind(&AirPlayNsdCore::didTimeout, 
		             this, 
		             boost::asio::placeholders::error));
}
