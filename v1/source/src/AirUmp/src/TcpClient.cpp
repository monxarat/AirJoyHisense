/*
* TcpClient.cpp
*
* coding.tom@gmail.com
*
*
* 2012-6-15
*/

#include "TcpClient.hpp"

using namespace airjoy;
using namespace boost::asio;

TcpClient::TcpClient(boost::asio::io_service &ios, QueuePointer q)
     : m_timer(ios)
     , m_socket(ios)
     , m_strand(ios)
     , m_queue(q)
     , m_isWaittingResponse(false)
     , m_timeoutForResponse(0)
     , m_isConnected(false)
{
    //std::cout << "TcpClient::TcpClient()" << std::endl;
}

TcpClient::~TcpClient()
{
    //std::cout << "TcpClient::~TcpClient()" << std::endl;
}

TcpClient::TcpSocketType & TcpClient::socket()
{
    return m_socket;
}

TcpClient::IosType & TcpClient::io_service()
{
    return m_socket.get_io_service();
}

bool TcpClient::isConnected(void)
{
    return m_isConnected;
}

void TcpClient::close()
{
    //std::cout << "TcpClient::close" << std::endl;

    if (! m_isConnected)
        return;

    m_isConnected = false;

    boost::system::error_code ignored_ec;
    m_socket.shutdown(ip::tcp::socket::shutdown_both, ignored_ec);
    m_socket.close(ignored_ec);

    // �Ͽ����ӣ�����Ϣ���з���һ����Ϣ
    TcpClientMessagePointer msg = boost::make_shared<TcpClientMessage>();
    msg->setType(TcpClientMessage::DisConnect);
    msg->setIp(m_serverIp);
    m_queue->push(msg);
}

void TcpClient::startConnectAndSend(const std::string &ip, int port, TcpClientMessagePointer msgWillWrite, int timeoutForResponse)
{
    //std::cout << "TcpClient::startConnectAndSend" << std::endl;
        
    this->startTimerIfNecessary(timeoutForResponse);

    m_messageToBeSend = msgWillWrite->message();
        
    try
    {
        TcpType::endpoint ep(boost::asio::ip::address::from_string(ip), port);

        m_socket.async_connect(ep,
            m_strand.wrap(
                boost::bind(&TcpClient::didConnectAndSend, 
                shared_from_this(),
                boost::asio::placeholders::error,
                msgWillWrite)
                )
            );
    }
    catch (std::exception &e)
    {
        //std::cout << "TcpClient::startWrite -> " << e.what() << std::endl;
    }
}

void TcpClient::startConnect(const std::string &ip, int port, int timeoutForResponse)
{
	//std::cout << "TcpClient::startConnect -> " << ip << " port: " << port << std::endl;

    this->startTimerIfNecessary(timeoutForResponse);

    try
    {
        TcpType::endpoint ep(boost::asio::ip::address::from_string(ip), port);

		// ������������
        m_socket.async_connect(ep,
            m_strand.wrap(
                boost::bind(&TcpClient::didConnect, 
                shared_from_this(),
                boost::asio::placeholders::error)
                )
            );
	}
    catch (std::exception &e)
    {
        //std::cout << "TcpClient::startConnect -> " << e.what() << std::endl;
    }
}

void TcpClient::didConnectAndSend(const boost::system::error_code &error, TcpClientMessagePointer msgWillWrite)
{
    //std::cout << "TcpClient::didConnectAndSend" << std::endl;
    
    // ֹͣ��ʱ��
    this->stopTimer();

    if (error)
    {
        if (m_isWaittingResponse)
        {
            // ����ʧ�ܣ�����Ϣ���з���һ����Ϣ���������
            TcpClientMessagePointer msg = boost::make_shared<TcpClientMessage>();
            msg->setType(TcpClientMessage::ConnectFailed);
            msg->setIp(m_serverIp);
            msg->message() = msgWillWrite->message();
            m_queue->push(msg);
        }

        this->close();
        return;
    }

    m_isConnected = true;

    // ��������
    startWrite(msgWillWrite);
    
    // ��ʼ�첽������
    TcpClientMessagePointer msgWillRead = boost::make_shared<TcpClientMessage>();
    startRead(msgWillRead);
}

void TcpClient::didConnect(const boost::system::error_code &error)
{
	//std::cout << "TcpClient::didConnect -> " << std::endl;
        
    // ֹͣ��ʱ��
    this->stopTimer();

    if (error)
    {
        if (m_isWaittingResponse)
        {
            // ����ʧ�ܣ�����Ϣ���з���һ����Ϣ���������
            TcpClientMessagePointer msg = boost::make_shared<TcpClientMessage>();
            msg->setType(TcpClientMessage::ConnectFailed);
            msg->setIp(m_serverIp);
            m_queue->push(msg);
        }

        this->close();
        return;
    }
    
    m_isConnected = true;

    // ���ӳɹ�������Ϣ���з���һ����Ϣ
    TcpClientMessagePointer msg = boost::make_shared<TcpClientMessage>();
    msg->setType(TcpClientMessage::ConnectOk);
    msg->setIp(m_serverIp);
    m_queue->push(msg);
    
    // ��ʼ�첽������
    TcpClientMessagePointer msgWillRead = boost::make_shared<TcpClientMessage>();
    startRead(msgWillRead);
}

void TcpClient::startWrite(TcpClientMessagePointer msgWillWrite, int timeoutForResponse)
{
    //std::cout << "TcpClient::startWrite" << std::endl;

    this->startTimerIfNecessary(timeoutForResponse);

    m_bufferForWrite.append(msgWillWrite->message().c_str(), msgWillWrite->message().size());

    try
    {
        m_socket.async_write_some(m_bufferForWrite.data(),
            m_strand.wrap(
                bind(&TcpClient::didWrite,
                shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred,
                msgWillWrite)
                )
            );
    }
    catch (std::exception &e)
    {
        // ֹͣ��ʱ��
        this->stopTimer();

        //std::cout << "TcpClient::startWrite -> " << e.what() << std::endl;
    }
}

void TcpClient::didWrite(const boost::system::error_code &error,
                         size_t bytes_transferred,
                         TcpClientMessagePointer msgDidWrite)
{
    //std::cout << "TcpClient::didWrite" << std::endl;
    
    // ֹͣ��ʱ��
    this->stopTimer();

    if (error)
    {
        // ���ڵȴ�Ӧ��
        if (m_isWaittingResponse)
        {
            // ����ʧ�ܣ�����Ϣ���з���һ����Ϣ
            TcpClientMessagePointer msg = boost::make_shared<TcpClientMessage>();
            msg->setType(TcpClientMessage::ErrorNet);
            msg->setIp(m_serverIp);
            msg->message() = msgDidWrite->message();
            m_queue->push(msg);
        }

        this->close();
        return;
    }

    m_bufferForWrite.consume(bytes_transferred);
}

void TcpClient::startRead(TcpClientMessagePointer msgWillRead)
{
    //std::cout << "TcpClient::startRead" << std::endl;

    try
    {
        m_socket.async_read_some(m_bufferForRead.prepare(1024*50), 
            m_strand.wrap(
                boost::bind(&TcpClient::didRead, 
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred,
                    msgWillRead)
                    )
            );
    }
    catch (std::exception &e)
    {
        // ֹͣ��ʱ��
        this->stopTimer();
        //std::cout << "TcpClient::startRead -> " << e.what() << std::endl;
    }
}

void TcpClient::didRead(const boost::system::error_code &error,
                        size_t bytes_transferred,
                        TcpClientMessagePointer msgDidRead)
{
	//std::cout << "TcpClient::didRead" << std::endl;
    
    // ֹͣ��ʱ��
    this->stopTimer();

    if (error)
    {
        // ���ڵȴ�Ӧ��
        if (m_isWaittingResponse)
        {
            // ����Ӧ��ʧ�ܣ�����Ϣ���з���һ������ʧ����Ϣ
            TcpClientMessagePointer msg = boost::make_shared<TcpClientMessage>();
            msg->setType(TcpClientMessage::ErrorNet);
            msg->setIp(m_serverIp);
            msg->message() = m_messageToBeSend;
            m_queue->push(msg);
        }

        this->close();
        return;
    }

    // ����Ӧ��
    m_bufferForRead.retrieve(bytes_transferred);

    msgDidRead->setType(TcpClientMessage::NormalMessage);
    msgDidRead->setIp(m_serverIp);
    msgDidRead->message().append(m_bufferForRead.peek(), bytes_transferred);

    m_bufferForRead.consume(bytes_transferred);
    m_queue->push(msgDidRead);

     // continue read
    TcpClientMessagePointer msgWillRead = boost::make_shared<TcpClientMessage>();
    startRead(msgWillRead);
}

void TcpClient::setServerIp(const std::string &ip)
{
    m_serverIp = ip;
}

const std::string & TcpClient::serverIp(void)
{
    return m_serverIp;
}

void TcpClient::stopTimer(void)
{
    m_isWaittingResponse = false;

    boost::system::error_code ec;
    m_timer.cancel(ec);
}

void TcpClient::startTimerIfNecessary(int timeoutForResponse)
{
    this->stopTimer();

    m_timeoutForResponse = timeoutForResponse;
    m_isWaittingResponse =  (m_timeoutForResponse > 0);

    try
    {
        // ������ʱ��ʱ��
        if (m_timeoutForResponse > 0)
        {
            m_timer.expires_from_now(boost::posix_time::seconds(m_timeoutForResponse));
            m_timer.async_wait(boost::bind(&TcpClient::didTimeout, shared_from_this(), boost::asio::placeholders::error));
        }
    }
    catch (std::exception &e)
    {
        boost::system::error_code ec;
        m_timer.cancel(ec);

        //std::cout << "TcpClient::startRead -> " << e.what() << std::endl;
    }
}

void TcpClient::didTimeout(const boost::system::error_code& error)
{
    //std::cout << "TcpClient::didTimeout" << std::endl;

    // ���ڵȴ�Ӧ��
    if (m_isWaittingResponse)
    {
        // ����Ӧ��ʧ�ܣ�����Ϣ���з���һ������ʧ����Ϣ
        TcpClientMessagePointer msg = boost::make_shared<TcpClientMessage>();
        msg->setType(TcpClientMessage::Timeout);
        msg->setIp(m_serverIp);
        msg->message() = m_messageToBeSend;
        m_queue->push(msg);

        // ȡ���������Ļص�
        m_isWaittingResponse = false;

        //this->close();
    }
}
