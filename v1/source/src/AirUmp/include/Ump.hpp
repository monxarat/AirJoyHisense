/*
 * Ump.hpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-8
 *
 */

#ifndef __UMP_H__
#define __UMP_H__

#include "Udp.hpp"

namespace airjoy
{

    class UmpHandler;

    class Ump : public boost::enable_shared_from_this<Ump>
    {
    public:
        typedef boost::shared_ptr<Udp>                  UdpPointer;
        typedef boost::shared_ptr<UmpHandler>           UmpHandlerPointer;

    private:
        UdpPointer                  m_pUdp;
        UmpHandlerPointer           m_handler;
        boost::asio::io_service   & m_ios;
        int                         m_port;
        bool                        m_isStart;

    public:
        Ump(boost::asio::io_service &ios, int port);
        virtual ~Ump();

        void start();
        void stop();
        int port();

        void setHandler(UmpHandlerPointer handler);

        bool send(const std::string &ip, int port, const std::string &message);

    private:
        bool didReceiveMessage(UdpMessagePointer message);
    };

}

#endif // __UMP_H__
