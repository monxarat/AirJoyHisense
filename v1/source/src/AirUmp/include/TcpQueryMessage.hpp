/*
 * TcpQueryMessage.hpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-8
 *
 */

#ifndef __TCP_QUERY_MESSAGE_H__
#define __TCP_QUERY_MESSAGE_H__

#include "TheBoost.hpp"
#include <string>

namespace airjoy
{

    class TcpQueryMessage : boost::noncopyable
    {
    private:
        std::string         m_ip;
        uint16_t            m_port;
        std::string         m_message;
        int                 m_timeout;
        bool                m_waitForReqponse;

    public:
        TcpQueryMessage()
            : m_port(0)
			, m_timeout(0)
            , m_waitForReqponse(false)
        {
        }
        
        TcpQueryMessage(const std::string &ip, uint16_t port, const std::string &msg, int timeout, bool waitForReqponse)
            : m_ip(ip)
            , m_port(port)
            , m_message(msg)
            , m_timeout(timeout)
            , m_waitForReqponse(waitForReqponse)
        {
        }

        virtual ~TcpQueryMessage()
        {
        }

        void setIp(const std::string &ip)
        {
            m_ip = ip;
        }

        const std::string & ip(void)
        {
            return m_ip;
        }

        void setPort(uint16_t port)
        {
            m_port = port;
        }

        uint16_t port(void)
        {
            return m_port;
        }
        
        void setMessage(const std::string &msg)
        {
            m_message = msg;
        }

        const std::string & message(void)
        {
            return m_message;
        }
                
        void setTimeout(int timeout)
        {
            m_timeout = timeout;
        }

        int timeout(void)
        {
            return m_timeout;
        }

        bool waitForReqponse(void)
        {
            return m_waitForReqponse;
        }
    };

}

#endif // __TCP_QUERY_MESSAGE_H__
