/*
* UmpServer.cpp
*
* coding.tom@gmail.com
*
* 2012-6-8
*
*/

#include "UmpServer.hpp"
#include "UmpServerHandler.hpp"
#include "AirJoyLog.hpp"
#include "TcpServer.hpp"
#include "TcpSession.hpp"
#include <exception>

using namespace airjoy;

UmpServer::UmpServer(boost::asio::io_service &ios, int port)
    : m_ios(ios)
    , m_port(port)
    , m_isStart(false)
{

}

UmpServer::~UmpServer()
{
}

void UmpServer::start()
{
    if (m_isStart)
        return;

    try
    {
        m_isStart = true;

        m_pMessageQ = boost::make_shared<QueueType>();

        m_pWorker = boost::make_shared<WorkerType>(m_pMessageQ);

        m_pServer = boost::make_shared<TcpServer>(boost::ref(m_ios), m_port, m_pMessageQ);
    
        m_pWorker->start(boost::bind(&UmpServer::didReceiveMessage, shared_from_this(), _1));

        m_pServer->start();
    }
    catch (std::exception &e)
    {
        LOGD("UmpServer::UmpServer() start failed");
        //LOGD(e.what());
    }
}

void UmpServer::stop()
{
    if (m_pServer.use_count() > 0)
        m_pServer->stop();

    if (m_pWorker.use_count() > 0)
        m_pWorker->stop();

    m_isStart = false;
}

int UmpServer::port()
{
    if (m_pServer.use_count() > 0)
        return m_pServer->port();

    return 0;
}

void UmpServer::setHandler(UmpServerHandlerPointer handler)
{
    m_handler = handler;
}

bool UmpServer::didReceiveMessage(TcpServerMessagePointer message)
{
    if (m_handler)
    {
        switch (message->messageType())
        {
        case TcpServerMessage::DidStart:
            this->addTcpSession(message->session());
            break;

        case TcpServerMessage::NormalMessage:
            if (message)
            {
                m_handler->didReceiveMessage(message);
            }
            return true;

#if 0
            if (! m_handler->didReceiveMessage(message))
            {
                message->session()->close();
            }
            break;
#endif

        case TcpServerMessage::DidClose:
            m_handler->didClose(message->session()->connectedId());
            this->removeTcpSession(message->session()->connectedId());
            break;

        default:
            break;
        }
    }

    return true;
}

bool UmpServer::send(int tcpSessionId, const std::string &message)
{
    TcpServer::TcpSessionPointer session;
    if (! this->getTcpSession(tcpSessionId, &session))
        return false;

    TcpServerMessagePointer request = boost::make_shared<TcpServerMessage>(session);
    request->message() = message;

    session->startWrite(request);

    return true;
}

bool UmpServer::closeSession(int tcpSessionId)
{
    TcpServer::TcpSessionPointer session;
    if (! this->getTcpSession(tcpSessionId, &session))
        return false;
    
    session->close();

    return true;
}

void UmpServer::addTcpSession(TcpSessionPointer session)
{
    //std::cout << "UmpServer::addTcpSession" << session->connectedId() << std::endl;

    LockType lock(m_tcpSessionsMutex);
    m_tcpSessions.insert(std::make_pair(session->connectedId(), session));
}

void UmpServer::removeTcpSession(int connectedId)
{
    //std::cout << "UmpServer::removeTcpSession" << connectedId << std::endl;

    LockType lock(m_tcpSessionsMutex);
    TcpSessionMap::iterator iter = m_tcpSessions.find(connectedId);
    if (iter != m_tcpSessions.end())
        m_tcpSessions.erase(iter);
}

bool UmpServer::getTcpSession(int connectedId, TcpSessionPointer *p)
{
    //std::cout << "UmpServer::getTcpSession" << connectedId << std::endl;

    LockType lock(m_tcpSessionsMutex);

    if (m_tcpSessions.size() == 0)
        return false;

    TcpSessionMap::iterator iter = m_tcpSessions.find(connectedId);
    if (iter != m_tcpSessions.end())
    {
        *p = iter->second;

        return true;
    }

    return false;
}
