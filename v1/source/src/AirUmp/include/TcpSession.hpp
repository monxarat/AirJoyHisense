/*
 * TcpSession.hpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-8
 *
 */

#ifndef __TCP_SESSION_H__
#define __TCP_SESSION_H__

#include "NetBuffer.hpp"
#include "TcpServerMessage.hpp"
#include "JobQueue.hpp"

#include "TheBoost.hpp"

namespace airjoy
{
    class TcpServer;

    typedef boost::shared_ptr<TcpServerMessage>   TcpServerMessagePointer;
    
    class TcpSession : public boost::enable_shared_from_this<TcpSession>
    {
    public:
        typedef boost::shared_ptr<TcpServer>        TcpServerPointer;
        typedef boost::asio::ip::tcp::socket        TcpSocketType;
        typedef boost::asio::io_service             IosType;
        typedef IosType::strand                     StrandType;
        typedef JobQueue<TcpServerMessagePointer>   QueueType;
        typedef boost::shared_ptr<QueueType>        QueuePointer;
        typedef std::vector<unsigned char>          Bytes;

    private:
        TcpSocketType               m_socket;
        StrandType                  m_strand;
        QueuePointer                m_queue;
        NetBuffer                   m_bufferForRead;
        NetBuffer                   m_bufferForWrite;
        std::string                 m_remoteIp;
        std::string                 m_selfIp;
        Bytes                       m_peerIpAddressBytes;
        Bytes                       m_selfIpAddressBytes;
        int                         m_connectedId;
        TcpServerPointer            m_tcpServer;

    public:
        TcpSession(TcpServerPointer server, IosType & ios, QueuePointer q, int connectedId);
        virtual ~TcpSession();

        TcpSocketType & socket();
        IosType & io_service();

        int connectedId(void) { return m_connectedId; }
        
        void start();
        void close();
        void startWrite(TcpServerMessagePointer response);
        
        const std::string & getPeerIp(void);
        const std::string & getSelfIp(void);

        Bytes & getPeerIPV4Bytes(void);
        Bytes & getSelfIPV4Bytes(void);

        Bytes & getPeerIPV6Bytes(void);
        Bytes & getSelfIPV6Bytes(void);

    private:
        void closeSession(void);

        void startRead(void);
        void didRead(const boost::system::error_code &error,
            size_t bytes_transferred,
            TcpServerMessagePointer request);

        void didWrite(const boost::system::error_code &error,
            size_t bytes_transferred,
            TcpServerMessagePointer response);

        void didClose(void);
    };

}

#endif // __TCP_SESSION_H__
