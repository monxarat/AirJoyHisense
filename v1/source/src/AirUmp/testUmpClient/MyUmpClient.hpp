/*
* MyUmpClient.hpp
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

#ifndef __MYUMPCLIENT_H__
#define __MYUMPCLIENT_H__

#include "TheBoost.hpp"
#include "AirJoyTypeDef.hpp"

namespace airjoy
{

    class MyUmpClient
    {
    public:
        typedef boost::shared_ptr<UmpClient>            UmpCLientPointer;
        typedef boost::shared_ptr<boost::thread>        ThreadPointer;

    private:
        bool                            m_isStart;
        boost::asio::io_service         m_ios;
        ThreadPointer                   m_pThread;
        UmpClientPointer                m_pUmpClient;

    public:
        MyUmpClient()
        {

        }

        virtual ~AnyAdpServer()
        {
        }

        bool start(void)
        {
            if (m_pThread.use_count() > 0)
                return true;

            std::cout << "m_pThread.use_count: " << m_pThread.use_count() << std::endl;

            //if (m_threads.size() > 0)
            //    return true;

            try
            {
                m_pAdp->start();

#if 1
                m_pThread = boost::make_shared<boost::thread>(boost::bind(&boost::asio::io_service::run, &m_ios));

                //m_threads.create_thread(boost::bind(&boost::asio::io_service::run, &m_ios));
#else
                if (m_ios_thread == NULL)
                    m_ios_thread = new boost::thread(boost::bind(&boost::asio::io_service::run, &m_ios));

                m_threads.add_thread(m_ios_thread);
#endif
            }
            catch (std::exception &e)
            {
                std::cout << "AnyAdpServer::start: " << e.what() << std::endl;
                return false;
            }

            return true;
        }

        bool stop(void)
        {
            if (m_pThread.use_count() == 0)
                return true;

            std::cout << "m_pThread.use_count: " << m_pThread.use_count() << std::endl;

            //if (m_threads.size() == 0)
            //    return true;

            m_ios.stop();
            m_pAdp->stop();

            m_pThread->join();
            m_pThread.reset();
            std::cout << "m_pThread.use_count: " << m_pThread.use_count() << std::endl;

            m_ios.reset();

            //m_threads.join_all();
#if 0
            m_threads.remove_thread(m_ios_thread);
            delete m_ios_thread;
            m_ios_thread = NULL;
#endif
            return true;
        }

        bool run(void)
        {
            start();
            //m_threads.join_all();

            return true;
        }

        AdpPointer                  adp(void)           { return m_pAdp; }
    };


    
    class TestServer
    {
    public:
        typedef boost::shared_ptr<boost::thread>            ThreadPointer;

    private:
        boost::asio::io_service         m_ios;
        boost::asio::deadline_timer     m_timer;
        ThreadPointer                   m_pThread;

    public:
        TestServer()
            : m_timer(m_ios, boost::posix_time::seconds(1))
        {

        }

        virtual ~TestServer()
        {
        }

        bool start(void)
        {
            if (m_pThread.use_count() > 0)
                return true;

            std::cout << "start m_pThread.use_count: " << m_pThread.use_count() << std::endl;

            //if (m_threads.size() > 0)
            //    return true;

            try
            {
                m_timer.expires_from_now(boost::posix_time::seconds(1));
                m_timer.async_wait(boost::bind(&TestServer::didTimeout, this, boost::asio::placeholders::error));

#if 1
                m_pThread = boost::make_shared<boost::thread>(boost::bind(&boost::asio::io_service::run, &m_ios));

                //m_threads.create_thread(boost::bind(&boost::asio::io_service::run, &m_ios));
#else
                if (m_ios_thread == NULL)
                    m_ios_thread = new boost::thread(boost::bind(&boost::asio::io_service::run, &m_ios));

                m_threads.add_thread(m_ios_thread);
#endif
            }
            catch (std::exception &e)
            {
                std::cout << "AnyAdpServer::start: " << e.what() << std::endl;
                return false;
            }

            return true;
        }

        bool stop(void)
        {
            std::cout << "stop: m_pThread.use_count: " << m_pThread.use_count() << std::endl;
            if (m_pThread.use_count() == 0)
                return true;

            //if (m_threads.size() == 0)
            //    return true;

            m_ios.stop();

            m_pThread->join();
            m_pThread.reset();
            std::cout << "stop: m_pThread.use_count: " << m_pThread.use_count() << std::endl;
            
            m_ios.reset();

#if 0
            //m_threads.join_all();
            m_threads.remove_thread(m_ios_thread);
            delete m_ios_thread;
            m_ios_thread = NULL;
#endif
            return true;
        }

        bool run(void)
        {
            start();
            //m_threads.join_all();

            return true;
        }

    private:
        void didTimeout(const boost::system::error_code& error)
        {
            std::cout << "didTimeout" << std::endl;

            if (error)
            {
                std::cout << "didTimeout error" << std::endl;
                return;
            }

            try
            {
                m_timer.expires_from_now(boost::posix_time::seconds(1));
                m_timer.async_wait(boost::bind(&TestServer::didTimeout, this, boost::asio::placeholders::error));
            }
            catch (std::exception &e)
            {
                std::cout << "didTimeout -> " << e.what() << std::endl;
            }

        }
    };

}

#endif // __MYUMPCLIENT_H__