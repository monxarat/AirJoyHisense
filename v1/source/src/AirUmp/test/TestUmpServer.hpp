/*
* TestUmpServer.hpp
*
*
* coding.tom@gmail.com
*
* 2012-5-23
*
* @remark
*      set tabstop=4
*      set shiftwidth=4
*      set expandtab
*/

#ifndef __TEST_UMP_SERVER_H__
#define __TEST_UMP_SERVER_H__

#include "TheBoost.hpp"
#include "AirJoyTypeDef.hpp"
#include "UmpServer.hpp"

namespace airjoy
{

    class TestUmpServer
    {
    public:
        typedef boost::shared_ptr<boost::thread>            ThreadPointer;
        typedef boost::shared_ptr<UmpServer>                UmpServerPointer;

    private:
        boost::asio::io_service         m_ios;
        ThreadPointer                   m_pThread;
        UmpServerPointer                m_pUmpServer;

    public:
        TestUmpServer()
        {
            // Ump Server
            m_pUmpServer = boost::make_shared<UmpServer>(boost::ref(m_ios), 0);

            //m_pServerHandler = boost::make_shared<AirJoyServerHandler>();
            //m_pUmpServer->setHandler(m_pServerHandler);

            int mpPort = m_pUmpServer->port();
            std::cout << "Ump Service  Port: " << mpPort << std::endl;
        }

        virtual ~TestUmpServer()
        {
            stop();
        }

        bool start(void)
        {
            if (m_pThread.use_count() > 0)
                return true;

            try
            {
                m_pUmpServer->start();

                m_pThread = boost::make_shared<boost::thread>(boost::bind(&boost::asio::io_service::run, &m_ios));
            }
            catch (std::exception &e)
            {
                std::cout << "TestUmpServer::start: " << e.what() << std::endl;
                return false;
            }

            return true;
        }

        bool stop(void)
        {
            if (m_pThread.use_count() == 0)
                return true;

            m_pUmpServer->stop();

            m_ios.stop();

            m_pThread->join();
            m_pThread.reset();

            m_ios.reset();

            return true;
        }

        bool run(void)
        {
            start();
            m_pThread->join();

            return true;
        }
    };

}

#endif // __TEST_UMP_SERVER_H__
