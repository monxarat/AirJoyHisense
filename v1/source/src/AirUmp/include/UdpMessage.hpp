/*
 * UdpMessage.hpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-8
 *
 */

#ifndef __UDP_MESSAGE_H__
#define __UDP_MESSAGE_H__

#include "TheBoost.hpp"
#include <string>

namespace airjoy
{

    class Udp;

    class UdpMessage : boost::noncopyable
    {
    public:
        typedef boost::shared_ptr<Udp>         UdpPointer;

        typedef enum _MessageType
        {
            Undefined       = 0,
            DidStart        = 1,
            NormalMessage   = 2,
            DidClose        = 3,
        } MessageType;

    private:
        MessageType         m_type;
        UdpPointer          m_udp;

#if 0
        enum { MAX_LENGTH = 2048 };
        char                m_buf[MAX_LENGTH];
        char                m_bufSize;
#else
        std::string         m_message;
#endif

        std::string         m_remoteIp;
        int                 m_remotePort;

    public:
        UdpMessage(UdpPointer udp)
            : m_udp(udp)
            , m_remotePort(0)
            //, m_bufSize(0)
        {
        }

        UdpMessage()
            : m_remotePort(0)
        {
        }

        virtual ~UdpMessage()
        {
        }

        UdpPointer server(void)
        {
            return m_udp;
        }

#if 0
        char *buf(void)
        {
            return m_buf;
        }

        size_t bufSize(void)
        {
            return m_bufSize;
        }

        void setBuf(const char *buf, size_t size)
        {
            m_bufSize = size;

            memcpy(m_buf, buf, size);
        }
#else
        std::string & message()
        {
            return m_message;
        }
#endif

        const MessageType messageType()
        {
            return m_type;
        }
        
        void setType(MessageType t)
        {
            m_type = t;
        }

        std::string & remoteIp()
        {
            return m_remoteIp;
        }

        void setRemoteIp(const std::string &ip)
        {
            m_remoteIp = ip;
        }

        int remotePort()
        {
            return m_remotePort;
        }

        void setRemotePort(int port)
        {
            m_remotePort = port;
        }
    };

}

#endif // __UDP_MESSAGE_H__
