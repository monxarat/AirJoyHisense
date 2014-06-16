/*
 * Udp.hpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-8
 *
 */

#ifndef __UDP_PORT_H__
#define __UDP_PORT_H__

#include "AirJoyLog.hpp"
#include "NetBuffer.hpp"
#include "UdpMessage.hpp"

#include "TheBoost.hpp"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/functional/factory.hpp>

namespace airjoy
{

    typedef boost::shared_ptr<UdpMessage>           UdpMessagePointer;

    class Udp : public boost::enable_shared_from_this<Udp>
    {
    public:
        typedef boost::asio::io_service             IosType;
        typedef IosType::strand                     StrandType;
        typedef boost::asio::ip::udp                UdpType;
        typedef boost::function<bool(UdpMessagePointer)>  FuncType;

    private:
        bool                        m_isStart;
        UdpType::socket             m_socket;
        StrandType                  m_strand;
        int                         m_port;
        NetBuffer                   m_bufferForRead;
        NetBuffer                   m_bufferForWrite;
        UdpType::endpoint           m_remoteEndpoint;
        FuncType                    m_func;

    public:
        Udp(boost::asio::io_service &ios, unsigned short port)
            : m_socket(ios, UdpType::endpoint(UdpType::v4(), port))
            , m_strand(ios)
            , m_port(port)
            , m_isStart(false)
        {
        }

        virtual ~Udp()
        {
        }

        void start(FuncType func)
        {
            if (m_isStart)
                return;

            m_func = func;

            m_isStart = true;

            this->startRead();

            UdpMessagePointer startMessage = boost::make_shared<UdpMessage>(shared_from_this());
            startMessage->setType(UdpMessage::DidStart);
            m_func(startMessage);
        }

        void stop(void)
        {
            if (! m_isStart)
                return;

            m_isStart = false;
            
            boost::system::error_code ignored_ec;
            m_socket.close(ignored_ec);

            UdpMessagePointer startMessage = boost::make_shared<UdpMessage>(shared_from_this());
            startMessage->setType(UdpMessage::DidClose);
            m_func(startMessage);
        }

        int port(void)
        {
            m_port = m_socket.local_endpoint().port();
            return m_port;
        }

        void startWrite(UdpMessagePointer pMessage)
        {
            boost::asio::ip::address receiverAddress = boost::asio::ip::address::from_string(pMessage->remoteIp());
            UdpType::endpoint receiverEndPoint(receiverAddress, pMessage->remotePort());

            //m_bufferForWrite.append(pMessage->buf(), pMessage->bufSize());
            m_bufferForWrite.append(pMessage->message().c_str(), pMessage->message().length());

            m_socket.async_send_to(m_bufferForWrite.data(),
                                   receiverEndPoint,
                                   m_strand.wrap(
                                       boost::bind(&Udp::didWrite, 
                                           shared_from_this(),
                                           boost::asio::placeholders::error,
                                           boost::asio::placeholders::bytes_transferred,
                                           pMessage)
                                           )
                                   );
        }

         void startWrite(const std::string &ip, int port, const std::string &message)
         {
             boost::asio::ip::address receiverAddress = boost::asio::ip::address::from_string(ip);
             UdpType::endpoint receiverEndPoint(receiverAddress, port);

             m_bufferForWrite.append(message.c_str(), message.size());

             m_socket.async_send_to(m_bufferForWrite.data(),
                                    receiverEndPoint,
                                    m_strand.wrap(
                                        boost::bind(&Udp::didWrite, 
                                            shared_from_this(),
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred)
                                            )
                                    );
         }

    private:
        void startRead(void)
        {
            try
            {
                UdpMessagePointer message = boost::make_shared<UdpMessage>(shared_from_this());

                m_socket.async_receive_from(m_bufferForRead.prepare(1024*10), 
                                            m_remoteEndpoint, 
                                            m_strand.wrap(
                                                boost::bind(&Udp::didRead, 
                                                     shared_from_this(),
                                                     boost::asio::placeholders::error,
                                                     boost::asio::placeholders::bytes_transferred,
                                                     message,
                                                     m_remoteEndpoint)
                            )
                        );
            }
            catch (std::exception &e)
            {
                //std::cout << "TcpSession::startRead -> " << e.what() << std::endl;
            }
        }

        void didRead(const boost::system::error_code &error,
                     size_t bytes_transferred,
                     UdpMessagePointer message,
                     UdpType::endpoint &remoteEndPoint)
        {
            if (error)
            {
                return;
            }

            //std::cout << "Udp::didRead: " << bytes_transferred << std::endl;

            m_bufferForRead.retrieve(bytes_transferred);

            message->message().append(m_bufferForRead.peek(), bytes_transferred);
            //message->setBuf(m_bufferForRead.peek(), bytes_transferred);
            message->setType(UdpMessage::NormalMessage);
            
            boost::system::error_code ignored_ec;
            message->setRemoteIp(remoteEndPoint.address().to_string(ignored_ec));
            message->setRemotePort(remoteEndPoint.port());

            m_bufferForRead.consume(bytes_transferred);

            m_func(message);

            this->startRead();
        }

        void didWrite(const boost::system::error_code &error,
                      size_t bytes_transferred)
        {
            if (error)
                return;

            m_bufferForWrite.consume(bytes_transferred);
        }

        void didWrite(const boost::system::error_code &error,
                      size_t bytes_transferred,
                      UdpMessagePointer message)
        {
            if (error)
                return;

            m_bufferForWrite.consume(bytes_transferred);
        }
    };

}

#endif // __UDP_PORT_H__
