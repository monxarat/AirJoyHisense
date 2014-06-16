/*
 * TcpServer.hpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-8
 *
 */

#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include "TcpSession.hpp"
#include "AirJoyLog.hpp"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

namespace airjoy
{

    class TcpServer : public boost::enable_shared_from_this<TcpServer>
    {
    public:
        typedef boost::shared_ptr<TcpSession>       TcpSessionPointer;
        typedef TcpSession::IosType                 IosType;
        typedef TcpSession::QueueType               QueueType;
        typedef boost::shared_ptr<QueueType>        QueuePointer;
        typedef boost::asio::ip::tcp::acceptor      AcceptorType;
        typedef boost::asio::ip::tcp                TcpType;

    private:
        AcceptorType                m_acceptor;
        QueuePointer                m_queue;
        int                         m_connectedId; // begin at 10000
        int                         m_port;

    public:
        TcpServer(boost::asio::io_service &ios, unsigned short port, QueuePointer q)
            : m_acceptor(ios, TcpType::endpoint(TcpType::v4(), port))
            , m_queue(q)
            , m_connectedId(10000)
            , m_port(port)
        {
            //LOGD("TcpServer::TcpServer()");
        }
        
        virtual ~TcpServer()
        {
            //LOGD("TcpServer::~TcpServer()");
        }

        void start()
        {
            //LOGD("TcpServer::start()");

            if (! m_acceptor.is_open())
            {
                //std::cout << "TcpServer::start port:" << m_port << std::endl;
                
                try
                {
                    TcpType::endpoint endpoint(TcpType::v4(), m_port); 
                    m_acceptor.open(endpoint.protocol());
                    m_acceptor.set_option(AcceptorType::reuse_address(true));
                    m_acceptor.bind(endpoint);
                    m_acceptor.listen();
                }
                catch (std::exception &e)
                {
                    std::cout << "TcpServer::start: " << e.what() << std::endl;
                }
            }

            startAccept();
        }

        void stop()
        {
            //std::cout << "TcpServer::stop" << std::endl;
            
            try
            {
                if (m_acceptor.is_open())
                {
                    m_acceptor.cancel();
                    m_acceptor.close();
                }
            }
            catch (std::exception &e)
            {
                std::cout << "TcpServer::stop: " << e.what() << std::endl;
            }
        }
        
        int port()
        {
            m_port = m_acceptor.local_endpoint().port();
            return m_port;
        }

    private:
        void startAccept()
        {
            //std::cout << "TcpServer::startAccept" << std::endl;

            m_connectedId ++;
            
            // 这里有个小问题：stop之后，session不能自动释放，非得要程序结束时才释放。

            TcpSessionPointer session = boost::make_shared<TcpSession>(shared_from_this(), 
                                                                       boost::ref(m_acceptor.get_io_service()), 
                                                                       m_queue,
                                                                       m_connectedId);

            try
            {
                m_acceptor.async_accept(session->socket(),
                    boost::bind(&TcpServer::didAccept, 
                    this,
                    boost::asio::placeholders::error,
                    session));
            }
            catch (std::exception &e)
            {
                std::cout << "TcpServer::startAccept: " << e.what() << std::endl;
            }
        }

        void didAccept(const boost::system::error_code &error, TcpSessionPointer session)
        {
            //std::cout << "TcpServer::didAccept" << std::endl;

            if (error)
            {
                session->close();
                
                return;
            }

            startAccept();

            session->start();
        }
    };

}

#endif // __TCP_SERVER_H__
