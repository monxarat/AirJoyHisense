/*
 * TcpServerMessage.hpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-8
 *
 */

#ifndef __TCP_SERVER_MESSAGE_H__
#define __TCP_SERVER_MESSAGE_H__

#include "TheBoost.hpp"
#include <string>

namespace airjoy
{

    class TcpSession;

    class TcpServerMessage : boost::noncopyable
    {
    public:
        typedef boost::shared_ptr<TcpSession>           TcpSessionPointer;

        typedef enum _MessageType
        {
            Undefined       = 0,
            DidStart        = 1000,
            NormalMessage   = 2000,
            DidClose        = 3000,
        } MessageType;

    private:
        MessageType         m_type;
        TcpSessionPointer   m_pSession;
        std::string         m_message;

    public:
        TcpServerMessage(TcpSessionPointer s)
            : m_pSession(s)
        {
        }

        virtual ~TcpServerMessage()
        {
        }

        TcpSessionPointer session()
        {
            return m_pSession;
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
    };

}

#endif // __TCP_SERVER_MESSAGE_H__
