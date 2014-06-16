/*
* TcpClient.hpp
*
* coding.tom@gmail.com
*
*
* 2012-6-15
*
*/

#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include "NetBuffer.hpp"
#include "TcpClientMessage.hpp"
#include "JobQueue.hpp"

#include "TheBoost.hpp"


namespace airjoy
{

    class TcpClient : public boost::enable_shared_from_this<TcpClient>
    {
    public:
        typedef boost::shared_ptr<TcpClientMessage>     TcpClientMessagePointer;
        typedef boost::asio::ip::tcp::socket            TcpSocketType;
        typedef boost::asio::io_service                 IosType;
        typedef IosType::strand                         StrandType;
        typedef JobQueue<TcpClientMessagePointer>       QueueType;
        typedef boost::shared_ptr<QueueType>            QueuePointer;
        typedef boost::asio::ip::tcp                    TcpType;
        typedef boost::asio::deadline_timer             DeadlineTimer;

    private:
        DeadlineTimer       m_timer;
        TcpSocketType       m_socket;
        StrandType          m_strand;
        QueuePointer        m_queue;
        NetBuffer           m_bufferForRead;
        NetBuffer           m_bufferForWrite;
        bool                m_isWaittingResponse;
        std::string         m_messageToBeSend;
        int                 m_timeoutForResponse;
        bool                m_isConnected;
        std::string         m_serverIp;

    public:
        TcpClient(boost::asio::io_service &ios, QueuePointer q);
        virtual ~TcpClient();

        TcpSocketType & socket();
        IosType & io_service();

        bool isConnected(void);

        void close();
		
		void testTimeout(int second);

        void startConnectAndSend(const std::string &ip, int port, TcpClientMessagePointer msgWillWrite, int timeoutForResponse);
        void startConnect(const std::string &ip, int port, int timeoutForResponse);

        void startWrite(TcpClientMessagePointer msgWillWrite, int timeoutForResponse = 0);
        void startRead(TcpClientMessagePointer msgWillRead);

        void setServerIp(const std::string &ip);
        const std::string &serverIp(void);

    private:
        void stopTimer(void);
        void startTimerIfNecessary(int timeoutForResponse);

        void didTimeout(const boost::system::error_code& error);

        void didConnectAndSend(const boost::system::error_code &error, TcpClientMessagePointer msgWillWrite);
        void didConnect(const boost::system::error_code &error);

        void didWrite(const boost::system::error_code &error,
                      size_t bytes_transferred,
                      TcpClientMessagePointer msgDidWrite);

        void didRead(const boost::system::error_code &error,
                     size_t bytes_transferred,
                     TcpClientMessagePointer msgDidRead);
    };

}

#endif //  __TCP_CLIENT_H__
