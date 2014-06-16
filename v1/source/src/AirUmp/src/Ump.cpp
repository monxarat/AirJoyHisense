/*
* Ump.cpp
*
* coding.tom@gmail.com
*
* 2012-6-8
*
*/

#include "Ump.hpp"
#include "UmpHandler.hpp"

using namespace airjoy;


Ump::Ump(boost::asio::io_service &ios, int port)
    : m_ios(ios)
    , m_port(port)
    , m_isStart(false)
{
}

Ump::~Ump()
{
}

void Ump::start()
{
    if (m_isStart)
        return;

    try
    {
        m_isStart = true;

        m_pUdp = boost::make_shared<Udp>(boost::ref(m_ios), m_port);
        m_pUdp->start(boost::bind(&Ump::didReceiveMessage, shared_from_this(), _1));
    }
    catch (std::exception &e)
    {
        std::cout << "Ump::start: " << e.what() << std::endl;
    }
}

void Ump::stop()
{
    if (m_pUdp.use_count() > 0)
        m_pUdp->stop();

    m_isStart = false;
}

int Ump::port()
{
    if (m_pUdp.use_count() > 0)
        return m_pUdp->port();

    return 0;
}

void Ump::setHandler(UmpHandlerPointer handler)
{
    m_handler = handler;
}

bool Ump::send(const std::string &ip, int port, const std::string &message)
{
    if (m_pUdp.use_count() > 0)
    {
        m_pUdp->startWrite(ip, port, message);
        return true;
    }

    return false;
}

bool Ump::didReceiveMessage(UdpMessagePointer message)
{
    if (m_handler)
    {
        switch (message->messageType())
        {
        case UdpMessage::DidStart:
            m_handler->didStart();
            break;

        case UdpMessage::NormalMessage:
            m_handler->didReceiveMessage(message);
			break;

        case UdpMessage::DidClose:
            m_handler->didClose();
            break;

        default:
            break;
        }
    }

    return true;
}
