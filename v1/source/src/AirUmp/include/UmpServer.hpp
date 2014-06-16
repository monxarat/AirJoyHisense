/*
 * UmpServer.hpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-8
 *
 */

#ifndef __UMP_SERVER_H__
#define __UMP_SERVER_H__

#include "TheBoost.hpp"
#include "TcpServer.hpp"
#include "Worker.hpp"

namespace airjoy
{

    class UmpServerHandler;

    class UmpServer : public boost::enable_shared_from_this<UmpServer>
    {
    public:
        typedef TcpServer::QueueType                    QueueType;
        typedef boost::shared_ptr<QueueType>            QueuePointer;
        typedef Worker<QueueType>                       WorkerType;
        typedef boost::shared_ptr<WorkerType>           WorkerPointer;
        typedef boost::shared_ptr<TcpServer>            TcpServerPointer;
        typedef boost::shared_ptr<UmpServerHandler>     UmpServerHandlerPointer;
        typedef TcpServer::TcpSessionPointer            TcpSessionPointer;        
        typedef std::map<int, TcpSessionPointer>        TcpSessionMap;
        typedef boost::mutex                            MutexType;
        typedef MutexType::scoped_lock                  LockType;

    private:
        QueuePointer                m_pMessageQ;
        WorkerPointer               m_pWorker;
        TcpServerPointer            m_pServer;
        UmpServerHandlerPointer     m_handler;
        boost::asio::io_service   & m_ios;
        int                         m_port;
        bool                        m_isStart;

        TcpSessionMap               m_tcpSessions;
        MutexType                   m_tcpSessionsMutex;

        std::string                 m_debugInfo;

    public:
        UmpServer(boost::asio::io_service &ios, int port);
        virtual ~UmpServer();

        void start();
        void stop();
        int port();

        void setHandler(UmpServerHandlerPointer handler);

        // 服务端主动发送消息
        bool send(int tcpSessionId, const std::string &message);

        // 关闭会话
        bool closeSession(int tcpSessionId);

    private:
        bool didReceiveMessage(TcpServerMessagePointer message);

        void addTcpSession(TcpSessionPointer session);
        void removeTcpSession(int connectedId);
        bool getTcpSession(int connectedId, TcpSessionPointer *p);
    };

}


#endif // __UMP_SERVER_H__
