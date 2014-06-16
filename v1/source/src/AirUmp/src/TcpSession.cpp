/*
 * TcpSession.cpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-8
 *
 */

#include "TcpSession.hpp"
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include "AirJoyLog.hpp"
#include "TcpServer.hpp"

using namespace boost;
using namespace boost::asio;
using namespace airjoy;

TcpSession::TcpSession(TcpServerPointer server, IosType & ios, QueuePointer q, int connectedId)
    : m_tcpServer(server)
    , m_socket(ios)
    , m_strand(ios)
    , m_queue(q)
    , m_connectedId(connectedId)
{
    //std::cout << "TcpSession::TcpSession: " << m_connectedId << std::endl;
}

TcpSession::~TcpSession()
{
    //std::cout << "TcpSession::~TcpSession: " << m_connectedId << std::endl;
}

TcpSession::TcpSocketType & TcpSession::socket()
{
    return m_socket;
}

TcpSession::IosType & TcpSession::io_service()
{
    return m_socket.get_io_service();
}

void TcpSession::start()
{
    //std::cout << "TcpSession::start: " << m_connectedId << std::endl;

    TcpServerMessagePointer startMessage = boost::make_shared<TcpServerMessage>(shared_from_this());
    startMessage->setType(TcpServerMessage::DidStart);
    m_queue->push(startMessage);

    startRead();
}

void TcpSession::closeSession()
{
    try
    {
        boost::system::error_code ignored_ec;
        m_socket.shutdown(ip::tcp::socket::shutdown_both, ignored_ec);
        m_socket.close(ignored_ec);
    }
    catch (std::exception &e)
    {
        //std::cout << "TcpSession::closeSession -> " << e.what() << std::endl;
    }
}

void TcpSession::close()
{
    //std::cout << "TcpSession::close: " << m_connectedId << std::endl;

    this->closeSession();

    try
    {
        this->didClose();
    }
    catch (std::exception &e)
    {
        //std::cout << "TcpSession::close -> " << e.what() << std::endl;
    }
}

void TcpSession::startRead(void)
{
    //std::cout << "TcpSession::startRead: " << m_connectedId << std::endl;

    try
    {
        TcpServerMessagePointer request = boost::make_shared<TcpServerMessage>(shared_from_this());

        m_socket.async_read_some(m_bufferForRead.prepare(1024*10), 
            m_strand.wrap(
                boost::bind(&TcpSession::didRead, 
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred,
                    request)
                    )
            );
    }
    catch (std::exception &e)
    {
        //std::cout << "TcpSession::startRead -> " << e.what() << std::endl;
    }
}

void TcpSession::didRead(const boost::system::error_code &error,
        size_t bytes_transferred,
        TcpServerMessagePointer request)
{
    if (error)
    {
        //std::cout << "TcpSession::didRead error" << std::endl;

        closeSession();
        this->didClose();
        return;
    }

    //std::cout << "TcpSession::didRead ok" << std::endl;

    m_bufferForRead.retrieve(bytes_transferred);
    
    request->message().append(m_bufferForRead.peek(), bytes_transferred);
    request->setType(TcpServerMessage::NormalMessage);
    m_queue->push(request);

    m_bufferForRead.consume(bytes_transferred);

    startRead();
}

void TcpSession::startWrite(TcpServerMessagePointer response)
{
    // std::cout << "startWrite: " << response->message() << std::endl;

    m_bufferForWrite.append(response->message().c_str(), response->message().size());

    try
    {
        m_socket.async_write_some(m_bufferForWrite.data(),
            m_strand.wrap(
                bind(&TcpSession::didWrite,
                shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred,
                response)
                )
            );
    }
    catch (std::exception &e)
    {
        //std::cout << "TcpSession::startWrite -> " << e.what() << std::endl;
    }
}

void TcpSession::didWrite(const boost::system::error_code &error,
        size_t bytes_transferred,
        TcpServerMessagePointer response)
{
    if (error)
    {
        closeSession();
        this->didClose();
        return;
    }

    //std::cout << "TcpSession::didWrite! -> " << this->connectedId() << std::endl;

    m_bufferForWrite.consume(bytes_transferred);
}

const std::string & TcpSession::getPeerIp(void)
{
    if (! m_remoteIp.empty())
        return m_remoteIp;

    try
    {
        m_remoteIp = m_socket.remote_endpoint().address().to_string();
    }
    catch (std::exception &e)
    {
        //std::cout << "TcpSession::getPeerIp -> " << e.what() << std::endl;
    }

    return m_remoteIp;
}

const std::string & TcpSession::getSelfIp(void)
{
    if (! m_selfIp.empty())
        return m_selfIp;

    try
    {
        m_selfIp = m_socket.local_endpoint().address().to_string();
    }
    catch (std::exception &e)
    {
        //std::cout << "TcpSession::getPeerIp -> " << e.what() << std::endl;
    }

    return m_selfIp;
}

TcpSession::Bytes & TcpSession::getPeerIPV4Bytes(void)
{
    if (m_peerIpAddressBytes.size() > 0)
        return m_peerIpAddressBytes;

    try
    {
        boost::asio::ip::address_v4::bytes_type bytes = m_socket.remote_endpoint().address().to_v4().to_bytes();
        for (size_t i = 0; i < bytes.size(); ++i)
            m_peerIpAddressBytes.push_back(bytes.at(i));
    }
    catch (std::exception &e)
    {
        //std::cout << "TcpSession::getPeerIpBytes -> " << e.what() << std::endl;
    }

    return m_peerIpAddressBytes;
}

TcpSession::Bytes & TcpSession::getSelfIPV4Bytes(void)
{
    if (m_selfIpAddressBytes.size() > 0)
        return m_selfIpAddressBytes;

    try
    {
        boost::asio::ip::address_v4::bytes_type bytes = m_socket.local_endpoint().address().to_v4().to_bytes();
        for (size_t i = 0; i < bytes.size(); ++i)
            m_selfIpAddressBytes.push_back(bytes.at(i));
    }
    catch (std::exception &e)
    {
        //std::cout << "TcpSession::getSelfIpBytes -> " << e.what() << std::endl;
    }

    return m_selfIpAddressBytes;
}

TcpSession::Bytes & TcpSession::getPeerIPV6Bytes(void)
{
    if (m_peerIpAddressBytes.size() > 0)
        return m_peerIpAddressBytes;

    try
    {
        boost::asio::ip::address_v6::bytes_type bytes = m_socket.remote_endpoint().address().to_v6().to_bytes();
        for (size_t i = 0; i < bytes.size(); ++i)
            m_peerIpAddressBytes.push_back(bytes.at(i));
    }
    catch (std::exception &e)
    {
        //std::cout << "TcpSession::getPeerIpBytes -> " << e.what() << std::endl;
    }

    return m_peerIpAddressBytes;
}

TcpSession::Bytes & TcpSession::getSelfIPV6Bytes(void)
{
    if (m_selfIpAddressBytes.size() > 0)
        return m_selfIpAddressBytes;

    try
    {
        boost::asio::ip::address_v6::bytes_type bytes = m_socket.local_endpoint().address().to_v6().to_bytes();
        for (size_t i = 0; i < bytes.size(); ++i)
            m_selfIpAddressBytes.push_back(bytes.at(i));
    }
    catch (std::exception &e)
    {
        //std::cout << "TcpSession::getSelfIpBytes -> " << e.what() << std::endl;
    }

    return m_selfIpAddressBytes;
}

void TcpSession::didClose(void)
{
	//std::cout << "TcpSession::didClose" << std::endl;

    TcpServerMessagePointer closeMessage = boost::make_shared<TcpServerMessage>(shared_from_this());
    closeMessage->setType(TcpServerMessage::DidClose);
    m_queue->push(closeMessage);
}