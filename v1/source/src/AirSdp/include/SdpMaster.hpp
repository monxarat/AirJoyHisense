/*
* SdpMaster.hpp
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

#ifndef __SDP_MASTER_H__
#define __SDP_MASTER_H__

#include "TheBoost.hpp"
#include "SdpMasterHandler.hpp"
#include "SdpNetwork.hpp"

#include <iostream>

#if 0
#define SDP_ENABLE_MULTICAST
#endif

#if 0
#define SDP_DEBUG
#endif

namespace airjoy
{

    class SdpMaster
    {
    public:
        typedef boost::shared_ptr<SdpMasterHandler> SdpMasterHandlerPointer;

    public:
        SdpMaster(boost::asio::io_service &ios, int port)
            : m_socket(ios)
            , m_port(port)
            , m_publishTimer(ios, boost::posix_time::seconds(1))
            , m_discoverTimer(ios, boost::posix_time::seconds(2))
            , m_serviceTimeoutTimer(ios, boost::posix_time::seconds(3))
        {
        }

        virtual ~SdpMaster()
        {
        }

        void setHandler(SdpMasterHandlerPointer h)
        {
            m_handler = h;
        }

        bool start(void)
        {
        #ifdef SDP_ENABLE_MULTICAST
            initMulticast();
        #else
            initBroadcast();
        #endif // SDP_ENABLE_MULTICAST

            try
            {
                m_socket.async_receive_from(
                        boost::asio::buffer(m_data, MAX_LENGTH), 
                        m_senderEndpoint,
                        boost::bind(&SdpMaster::didReceiveFrom, 
                            this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));

                m_publishTimer.expires_from_now(boost::posix_time::seconds(1));
                m_publishTimer.async_wait(boost::bind(&SdpMaster::didPublishTimeout, this, boost::asio::placeholders::error));

                m_discoverTimer.expires_from_now(boost::posix_time::seconds(2));
                m_discoverTimer.async_wait(boost::bind(&SdpMaster::didDiscoverTimeout, this, boost::asio::placeholders::error));

                m_serviceTimeoutTimer.expires_from_now(boost::posix_time::seconds(3));
                m_serviceTimeoutTimer.async_wait(boost::bind(&SdpMaster::didServiceTimeout, this, boost::asio::placeholders::error));
            }
            catch (std::exception &e)
            {
                std::cout << "SdpMaster::start -> " << e.what() << std::endl;
                return false;
            }

            return true;
        }

        bool stop(void)
        {
            try
            {
                m_socket.close();
            }
            catch (std::exception &e)
            {
                std::cout << "SdpMaster::stop -> " << e.what() << std::endl;
                return false;
            }

            return true;
        }

        void setPort(int port)
        { 
            m_port = port; 
        }

        int port(void) 
        { 
            return m_port; 
        }

        void sendToAll(const std::string &message)
        {
        #ifdef SDP_ENABLE_MULTICAST
            multicast(message);
        #else
            broadcast(message);
        #endif // SDP_ENABLE_MULTICAST
        }

        void sendTo(const std::string &ip, int port, const std::string &message)
        {
            m_sendMessage = message;

            try
            {
                boost::asio::ip::address receiverAddress = boost::asio::ip::address::from_string(ip);
                boost::asio::ip::udp::endpoint receiverEndPoint(receiverAddress, port);

#ifdef SDP_DEBUG
                std::cout << "-----------------------------> send to ip: " << ip << std::endl;
                std::cout << m_sendMessage << std::endl;
#endif // SDP_DEBUG

                m_socket.async_send_to(boost::asio::buffer(m_sendMessage),
                        receiverEndPoint,
                        boost::bind(&SdpMaster::didSendTo, this, boost::asio::placeholders::error));
            }
            catch (std::exception &e)
            {
                std::cout << "SdpMaster::sendTo -> " << e.what() << std::endl;
            }
        }

    private:
        bool initBroadcast(void)
        {
            try
            {
                // listen address
                boost::asio::ip::address listenAddress(boost::asio::ip::address_v4::any());
                boost::asio::ip::udp::endpoint listenEndpoint(listenAddress, m_port);
                
                // open socket
                m_socket.open(listenEndpoint.protocol());

                // set socket option
                m_socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
                m_socket.set_option(boost::asio::socket_base::broadcast(true));
                
                // disable loopback (no copies of our packets)
                m_socket.set_option(boost::asio::ip::multicast::enable_loopback(false));

                // bind port
                m_socket.bind(listenEndpoint);
            }
            catch (std::exception &e)
            {
                std::cout << "SdpMaster::init -> " << e.what() << std::endl;
                return false;
            }

            return true;
        }

        bool initMulticast(void)
        {
            try
            {
                // multicast address
                boost::asio::ip::address multicastAddress(boost::asio::ip::address::from_string("239.255.255.250"));
                
                // listen address
                boost::asio::ip::address listenAddress(boost::asio::ip::address_v4::any());
                boost::asio::ip::udp::endpoint listenEndpoint(listenAddress, m_port);

                // open socket
                m_socket.open(listenEndpoint.protocol());

                // set socket option
                m_socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
                
                // disable loopback (no copies of our packets)
                m_socket.set_option(boost::asio::ip::multicast::enable_loopback(false));

                // bind port
                m_socket.bind(listenEndpoint);

                // Join the multicast group.
                m_socket.set_option(boost::asio::ip::multicast::join_group(multicastAddress));
            }
            catch (std::exception &e)
            {
                std::cout << "SdpMaster::init -> " << e.what() << std::endl;
                return false;
            }

            return true;
        }

        void broadcast(const std::string &message)
        {
    #if 1
            std::vector<std::string> list;
            SdpNetwork network;

            if (network.getBroadcastList(list))
                std::for_each(list.begin(), list.end(), boost::bind(&SdpMaster::sendTo, this, _1, m_port, message));
    #else
            m_sendMessage = message;
            m_socket.async_send_to(boost::asio::buffer(m_sendMessage), 
                                   boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::broadcast(), m_port), 
                                   boost::bind(&SdpMaster::didSendTo, this, boost::asio::placeholders::error));
    #endif
        }

        void multicast(const std::string &message)
        {
            m_sendMessage = message;

            boost::asio::ip::address multicastAddress(boost::asio::ip::address::from_string("239.255.255.250"));
            boost::asio::ip::udp::endpoint multicastEndpoint(multicastAddress, m_port);

            m_socket.async_send_to(boost::asio::buffer(m_sendMessage),
                multicastEndpoint,
                boost::bind(&SdpMaster::didSendTo, this, boost::asio::placeholders::error));
        }

        void didPublishTimeout(const boost::system::error_code& error)
        {
            if (error)
            {
                return;
            }

            if (m_handler.use_count() != 0)
            {
                m_handler->onPublishTimeout(this);
                long timeout = (long)m_handler->getPublishTimeout(this);
                if (timeout == 0)
                    timeout = 30;

                try
                {
                    m_publishTimer.expires_from_now(boost::posix_time::seconds(timeout));
                    m_publishTimer.async_wait(boost::bind(&SdpMaster::didPublishTimeout, this, boost::asio::placeholders::error));
                }
                catch (std::exception &e)
                {
                    std::cout << "SdpMaster::didPublishTimeout -> " << e.what() << std::endl;
                }
            }
        }

        void didDiscoverTimeout(const boost::system::error_code& error)
        {
            if (error)
            {
                return;
            }

            if (m_handler.use_count() != 0)
            {
                m_handler->onDiscoverTimeout(this);
                long timeout = (long)m_handler->getDiscoverTimeout(this);
                if (timeout == 0)
                    timeout = 30;

                try
                {
                    m_discoverTimer.expires_from_now(boost::posix_time::seconds(timeout));
                    m_discoverTimer.async_wait(boost::bind(&SdpMaster::didDiscoverTimeout, this, boost::asio::placeholders::error));
                }
                catch (std::exception &e)
                {
                    std::cout << "SdpMaster::didDiscoverTimeout -> " << e.what() << std::endl;
                }
            }
        }

        void didServiceTimeout(const boost::system::error_code& error)
        {
            if (error)
            {
                return;
            }

            if (m_handler.use_count() != 0)
            {
                m_handler->onServiceTimeout(this);
                long timeout = (long)m_handler->getServiceTimeout(this);
                if (timeout == 0)
                    timeout = 30;

                try
                {
                    m_serviceTimeoutTimer.expires_from_now(boost::posix_time::seconds(timeout));
                    m_serviceTimeoutTimer.async_wait(boost::bind(&SdpMaster::didServiceTimeout, this, boost::asio::placeholders::error));
                }
                catch (std::exception &e)
                {
                    std::cout << "SdpMaster::didServiceTimeout -> " << e.what() << std::endl;
                }
            }
        }

        void didReceiveFrom(const boost::system::error_code& error, size_t bytes_recvd)
        {
            if (error)
                return;

            if (m_handler.use_count() != 0)
            {
                std::string message(m_data, bytes_recvd);
    
#ifdef SDP_DEBUG
                std::cout << "------------- didReceive From:  ------------------: " << m_senderEndpoint.address().to_string() << std::endl;
                std::cout << message << std::endl;
#endif // SDP_DEBUG

				m_handler->onReceived(this, m_senderEndpoint.address().to_string(), m_senderEndpoint.port(), message);
            }

            try
            {
                m_socket.async_receive_from(
                        boost::asio::buffer(m_data, MAX_LENGTH), 
                        m_senderEndpoint,
                        boost::bind(&SdpMaster::didReceiveFrom, 
                            this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
            }
            catch (std::exception &e)
            {
                std::cout << "SdpMaster::didReceiveFrom -> " << e.what() << std::endl;
            }
        }

        void didSendTo(const boost::system::error_code& error)
        {
            if (error)
            {
                //std::cout << "SdpMaster::didSendTo error" << std::endl;
                return;
            }
        }

    private:
        boost::asio::ip::udp::socket        m_socket;
        boost::asio::ip::udp::endpoint      m_senderEndpoint;
        int                                 m_port;

        // timer
        boost::asio::deadline_timer         m_publishTimer;         // 发布服务定时器
        boost::asio::deadline_timer         m_discoverTimer;        // 搜索服务定时器
        boost::asio::deadline_timer         m_serviceTimeoutTimer;  // 服务超时定时器

        // buffer
        enum { MAX_LENGTH = 2048 };
        char                                m_data[MAX_LENGTH];

        // send message
        std::string                         m_sendMessage;

        // handler
        SdpMasterHandlerPointer             m_handler;
    };

}

#endif // __SDP_MASTER_H__
