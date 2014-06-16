/*
* AirPort.cpp
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

#include "AirPort.hpp"
#include "AirPortDataHandler.hpp"
#include "AirPortTimingHandler.hpp"
#include "AirPortControlHandler.hpp"

using namespace airjoy;

AirPort::AirPort()
    : m_isStart(false)
    , m_clientControlPort(0)
{
    //std::cout << "AirPort::AirPort()" << std::endl;

    m_pData = boost::make_shared<Ump>(boost::ref(m_ios), 0);
    m_pTiming = boost::make_shared<Ump>(boost::ref(m_ios), 0);
    m_pControl = boost::make_shared<Ump>(boost::ref(m_ios), 0);


    m_pDataHandler = boost::make_shared<AirPortDataHandler>();
    m_pTimingHandler = boost::make_shared<AirPortTimingHandler>();
    m_pControlHandler = boost::make_shared<AirPortControlHandler>();

    m_pData->setHandler(m_pDataHandler);
    m_pTiming->setHandler(m_pTimingHandler);
    m_pControl->setHandler(m_pControlHandler);
}

AirPort::~AirPort()
{
    //std::cout << "AirPort::~AirPort()" << std::endl;

    stop();
}

bool AirPort::start(void)
{
    if (m_isStart)
        return false;

    if (m_pThread.use_count() > 0)
        return false;

    try
    {
        m_pData->start();
        m_pTiming->start();
        m_pControl->start();

        m_pThread = boost::make_shared<boost::thread>(boost::bind(&boost::asio::io_service::run, &m_ios));

        //std::cout << "data port: " << m_pData->port() << std::endl;
        //std::cout << "timing port: " << m_pTiming->port() << std::endl;
        //std::cout << "control port: " << m_pControl->port() << std::endl;
    }
    catch (std::exception &e)
    {
        //std::cout << "AirPlayServerCore::start: " << e.what() << std::endl;
        return false;
    }

    m_isStart = true;

    return true;
}

bool AirPort::stop(void)
{
    if (! m_isStart)
        return false;

    if (m_pThread.use_count() == 0)
        return false;

    m_pData->stop();
    m_pTiming->stop();
    m_pControl->stop();

    m_ios.stop();

    m_pThread->join();
    m_pThread.reset();

    m_ios.reset();

    m_isStart = false;

    return true;
}

void AirPort::setBuffer(AudioBuffer *audioBuffer)
{
    m_pDataHandler->setBuffer(audioBuffer);
}

int AirPort::dataPort(void)
{
    return m_pData->port();
}

int AirPort::timingPort(void)
{
    return m_pTiming->port();
}

int AirPort::controlPort(void)
{
    return m_pControl->port();
}

void AirPort::setClientIp(const std::string &ip)
{
    m_ip = ip;
}

void AirPort::setClientControlPort(int port)
{
    m_clientControlPort = port;
}

void AirPort::sendRtcpRequest(const unsigned char *buf, size_t size)
{
    if (! m_isStart)
        return;

    std::string message((const char *)buf, size);

    m_pControl->send(m_ip, m_clientControlPort, message);
}