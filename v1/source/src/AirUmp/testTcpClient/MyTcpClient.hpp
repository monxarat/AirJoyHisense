/*
 * MyTcpClient.hpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-15
 *
 */

#ifndef __MY_TCP_CLIENT_H__
#define __MY_TCP_CLIENT_H__

#include "TheBoost.hpp"
#include "TcpClient.hpp"
#include "Worker.hpp"
#include "AirJoyTypeDef.hpp"

namespace airjoy
{

    //class UmpClientHandler;
    //typedef boost::shared_ptr<UmpClientHandler>         UmpClientHandlerPointer;

    class MyTcpClient
    {
        public:
            typedef TcpClient::QueueType                QueueType; 
            typedef TcpClient::QueuePointer             QueuePointer; 
            typedef TcpClient::TcpClientMessagePointer  TcpClientMessagePointer;
            typedef Worker<QueueType>                   WorkerType;
            typedef boost::shared_ptr<WorkerType>       WorkerPointer;
            typedef boost::shared_ptr<TcpClient>        TcpClientPointer;

			typedef boost::shared_ptr<boost::thread>                ThreadPointer;

        private:
            QueuePointer                m_pMessageQ;
            WorkerPointer               m_pWorker;
            TcpClientPointer            m_pClient;
            boost::asio::io_service     m_ios;
            bool                        m_isStart;

			ThreadPointer                   m_pThread;

        public:
            MyTcpClient() 
                : m_isStart(false)
            {}

            virtual ~MyTcpClient() 
            {
				stop();
			}

			void start()
			{
				startClient();
				
				//startThread();
			}

			void stop()
			{
				//stopThread();
				stopClient();
			}

            //void setHandler(UmpClientHandlerPointer handler);
			void testTimeout(int s)
			{
				m_pClient = boost::make_shared<TcpClient>(boost::ref(m_ios), m_pMessageQ);
				m_pClient->testTimeout(s);

				m_ios.run();
			}

            void startRequest(const std::string &ip, int port, const std::string msg, int timeout)
            {
                std::cout << "startRequest" << std::endl;

				TcpClientMessagePointer pMessage = boost::make_shared<TcpClientMessage>();
				pMessage->setType(TcpClientMessage::NormalMessage);
				pMessage->message() = msg;

                m_pClient = boost::make_shared<TcpClient>(boost::ref(m_ios), m_pMessageQ);
                m_pClient->startConnectAndSend(ip, port, pMessage, timeout);

				m_ios.run();
            }

            void connect(const std::string &ip, int port, int timeout)
            {
                std::cout << "connect" << std::endl;
                
                m_pClient = boost::make_shared<TcpClient>(boost::ref(m_ios), m_pMessageQ);
                m_pClient->startConnect(ip, port, timeout);

				m_ios.run();
            }

            void disconnect(void)
            {
                std::cout << "disconnect" << std::endl;
                
                if (m_pClient.use_count() > 0)
                {
                    m_pClient->close();
                    m_pClient.reset();
                }
            }

            void send(TcpClientMessagePointer request, int timeout)
            {
                std::cout << "send" << std::endl;
                
                if (m_pClient.use_count() > 0)
                {
                    m_pClient->startWrite(request, timeout);
                }
            }

            bool isConnected(void)
            {
                std::cout << "isConnected" << std::endl;
                
                if (m_pClient.use_count() == 0)
                    return false;

                return m_pClient->isConnected();
            }

        private:
            bool didReceiveMessage(TcpClientMessagePointer message)
            {
                std::cout << "didReceiveMessage" << std::endl;

                return true;
            }
			            
			void startClient()
            {
                std::cout << "startClient" << std::endl;
                
                if (m_isStart)
                    return;

                m_pMessageQ = boost::make_shared<QueueType>();

                m_pWorker = boost::make_shared<WorkerType>(m_pMessageQ);

                m_pWorker->start(boost::bind(&MyTcpClient::didReceiveMessage, this, _1));
            }
			
            void stopClient()
            {
                std::cout << "stopClient" << std::endl;

                if (m_pWorker.use_count() > 0)
                    m_pWorker->stop();

                m_isStart = false;
            }

			bool startThread(void)
			{
				std::cout << "startThread" << std::endl;

				if (m_isStart)
					return false;

				if (m_pThread.use_count() > 0)
					return false;

				try
				{
					m_pThread = boost::make_shared<boost::thread>(boost::bind(&boost::asio::io_service::run, &m_ios));
				}
				catch (std::exception &e)
				{
					std::cout << "startThread exception" << std::endl;

					return false;
				}

				m_isStart = true;

				return true;
			}

			bool stopThread(void)
			{
				std::cout << "stopThread" << std::endl;

				if (! m_isStart)
					return false;

				if (m_pThread.use_count() == 0)
					return false;

				m_ios.stop();

				m_pThread->join();
				m_pThread.reset();

				m_ios.reset();

				m_isStart = false;

				return true;
			}
    };

}

#endif // __MY_TCP_CLIENT_H__
