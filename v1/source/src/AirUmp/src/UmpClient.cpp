/*
* UmpClient.cpp
*
* coding.tom@gmail.com
*
* 2012-6-8
*
*/

#include "UmpClient.hpp"
#include "UmpClientHandler.hpp"

using namespace airjoy;

UmpClient::UmpClient(boost::asio::io_service &ios)
    : m_ios(ios)
    , m_isStart(false)
{
}

UmpClient::~UmpClient()
{
}

void UmpClient::start()
{
    if (m_isStart)
        return;

    // receiver worker
    m_pReceiverMessageQ = boost::make_shared<QueueType>();
    m_pReceiverWorker = boost::make_shared<ReceiverWorkerType>(m_pReceiverMessageQ);
    m_pReceiverWorker->start(boost::bind(&UmpClient::didReceiveMessage, shared_from_this(), _1));

    // sender worker
    m_pSenderMessageQ = boost::make_shared<QueryQueueType>();
    m_pSenderWorker = boost::make_shared<SenderWorkerType>(m_pSenderMessageQ);
    m_pSenderWorker->start(boost::bind(&UmpClient::willSendMessage, shared_from_this(), _1));

    m_isStart = true;
}

void UmpClient::stop()
{
    if (m_pReceiverWorker.use_count() > 0)
        m_pReceiverWorker->stop();

    if (m_pSenderWorker.use_count() > 0)
        m_pSenderWorker->stop();

    m_isStart = false;
}

void UmpClient::setHandler(UmpClientHandlerPointer handler)
{
    m_handler = handler;
}

void UmpClient::connect(const std::string &ip, int port, int timeout)
{
    m_pClient = boost::make_shared<TcpClient>(boost::ref(m_ios), m_pReceiverMessageQ);
    m_pClient->startConnect(ip, port, timeout);
    m_pClient->setServerIp(ip);
}

void UmpClient::disconnect(void)
{
    if (m_pClient.use_count() > 0)
    {
        m_pClient->close();
        m_pClient.reset();
    }
}

bool UmpClient::isConnected(void)
{
    if (m_pClient.use_count() == 0)
        return false;

    return m_pClient->isConnected();
}

bool UmpClient::send(const std::string &ip, uint16_t port, const std::string & message, int timeout)
{
    TcpQueryMessagePointer msg = boost::make_shared<TcpQueryMessage>(ip, port, message, timeout, true);
    m_pSenderMessageQ->push(msg);

    return true;
}

bool UmpClient::send(const std::string & message, int timeout)
{
    if (this->isConnected())
        return this->send("", 0, message, timeout);

    return false;
}

bool UmpClient::didReceiveMessage(TcpClientMessagePointer message)
{
    this->receiveResponse();

    if (m_handler)
        m_handler->didReceiveMessage(message);

    return true;
}

bool UmpClient::willSendMessage(TcpQueryMessagePointer m)
{
    TcpClientMessagePointer pMessage = boost::make_shared<TcpClientMessage>();
    pMessage->setType(TcpClientMessage::NormalMessage);
    pMessage->message() = m->message();

    if (m->port() > 0)
    {
        m_pClient = boost::make_shared<TcpClient>(boost::ref(m_ios), m_pReceiverMessageQ);
        m_pClient->startConnectAndSend(m->ip(), m->port(), pMessage, m->timeout());
        m_pClient->setServerIp(m->ip());
    }
    else
    {
        if (m_pClient.use_count() == 0)
            m_pClient = boost::make_shared<TcpClient>(boost::ref(m_ios), m_pReceiverMessageQ);

        m_pClient->startWrite(pMessage, m->timeout());
    }

    // 如果此请求需要等待应答
    if (m->waitForReqponse())
        this->waitForResponse();

    return true;
}

void UmpClient::waitForResponse(void)
{
    LockType lock(m_mutex);
    m_hasResponse.wait(m_mutex);
}

void UmpClient::receiveResponse(void)
{
    LockType lock(m_mutex);
    m_hasResponse.notify_one();
}

const std::string & UmpClient::serverIp(void)
{
    return m_pClient->serverIp();
}