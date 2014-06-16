/*
 * TcpClientMessage.hpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-8
 *
 */

#ifndef __TCP_CLIENT_MESSAGE_H__
#define __TCP_CLIENT_MESSAGE_H__

#include "TheBoost.hpp"
#include <string>

namespace airjoy
{

    class TcpClientMessage : boost::noncopyable
    {
    public:
        typedef enum _MessageType
        {
            Undefined       = 0,
            NormalMessage   = 1,
            Timeout         = 2,
            ConnectOk       = 3,
            ConnectFailed   = 4,
            DisConnect      = 5,
            ErrorNet        = 6,
        } MessageType;

    private:
        MessageType         m_type;
        std::string         m_message;
        std::string         m_serverIp;

    public:
        TcpClientMessage()
            : m_type(Undefined)
        {
        }

        virtual ~TcpClientMessage()
        {
        }

        std::string & message()
        {
            return m_message;
        }

        const MessageType messageType()
        {
            return m_type;
        }
        
        void setType(MessageType t)
        {
            m_type = t;
        }

        const std::string & serverIp(void)
        {
            return m_serverIp;
        }

        void setIp(const std::string &ip)
        {
            m_serverIp = ip;
        }
    };

}

#endif // __TCP_CLIENT_MESSAGE_H__
