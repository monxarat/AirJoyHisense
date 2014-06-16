/*
* UmpClient.hpp
*
* coding.tom@gmail.com
*
* 2012-6-15
*
*/

#ifndef __UMP_CLIENT_H__
#define __UMP_CLIENT_H__

#include "TheBoost.hpp"
#include "TcpClient.hpp"
#include "Worker.hpp"
#include "AirJoyTypeDef.hpp"
#include "TcpQueryMessage.hpp"

namespace airjoy
{

    class UmpClientHandler;
    typedef boost::shared_ptr<UmpClientHandler>         UmpClientHandlerPointer;

    class UmpClient : public boost::enable_shared_from_this<UmpClient>
    {
    public:
        typedef boost::shared_ptr<TcpClient>            TcpClientPointer;

        // receiver worker & queue
        typedef TcpClient::QueueType                    QueueType; 
        typedef TcpClient::QueuePointer                 QueuePointer; 
        typedef TcpClient::TcpClientMessagePointer      TcpClientMessagePointer;
        typedef Worker<QueueType>                       ReceiverWorkerType;
        typedef boost::shared_ptr<ReceiverWorkerType>   ReceiverWorkerPointer;

        // sender worker & queue
        typedef boost::shared_ptr<TcpQueryMessage>      TcpQueryMessagePointer;
        typedef JobQueue<TcpQueryMessagePointer>        QueryQueueType;
        typedef boost::shared_ptr<QueryQueueType>       QueryQueuePointer;
        typedef Worker<QueryQueueType>                  SenderWorkerType;
        typedef boost::shared_ptr<SenderWorkerType>     SenderWorkerPointer;

        // wait for response
        typedef boost::mutex                            MutexType;
        typedef MutexType::scoped_lock                  LockType;
        typedef boost::condition_variable_any           ConditionType;

    private:
        TcpClientPointer            m_pClient;
        UmpClientHandlerPointer     m_handler;
        boost::asio::io_service   & m_ios;
        bool                        m_isStart;

        // receiver worker & queue
        QueuePointer                m_pReceiverMessageQ;
        ReceiverWorkerPointer       m_pReceiverWorker;

        // sender worker & queue
        QueryQueuePointer           m_pSenderMessageQ;
        SenderWorkerPointer         m_pSenderWorker;
        
        // wait for response 
        MutexType                   m_mutex;
        ConditionType               m_hasResponse;

    public:
        UmpClient(boost::asio::io_service &ios);
        virtual ~UmpClient();

        void start();
        void stop();
        void setHandler(UmpClientHandlerPointer handler);

        void connect(const std::string &ip, int port, int timeout);
        void disconnect(void);
        bool isConnected(void);

        bool send(const std::string &ip, uint16_t port, const std::string &message, int timeout);
        bool send(const std::string &message, int timeout);

        const std::string &serverIp(void);

    private:
        bool didReceiveMessage(TcpClientMessagePointer message);
        bool willSendMessage(TcpQueryMessagePointer message);
        
        void waitForResponse(void);
        void receiveResponse(void);
    };

}

#endif // __UMP_CLIENT_H__
