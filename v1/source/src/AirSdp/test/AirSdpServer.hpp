/*
* AirSdpServer.hpp
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

#ifndef __AIRSDP_SERVER_H__
#define __AIRSDP_SERVER_H__

#include "TheBoost.hpp"
#include "AirJoyTypeDef.hpp"
#include "Sdp.hpp"

namespace airjoy
{

    class AirSdpServer
    {
    public:
        typedef boost::shared_ptr<Sdp>                      SdpPointer;
        typedef boost::shared_ptr<boost::thread>            ThreadPointer;

    private:
        boost::asio::io_service         m_ios;
        ThreadPointer                   m_pThread;
        SdpPointer                      m_pSdp;

    public:
        AirSdpServer()
        {
             // Auto Discovery
             m_pSdp = boost::make_shared<Sdp>(boost::ref(m_ios), 1900);
        }

        virtual ~AirSdpServer()
        {
        }

        bool start(void)
        {
            if (m_pThread.use_count() > 0)
                return true;

            try
            {
                m_pSdp->start();

                m_pThread = boost::make_shared<boost::thread>(boost::bind(&boost::asio::io_service::run, &m_ios));
            }
            catch (std::exception &e)
            {
                std::cout << "AirSdpServer::start: " << e.what() << std::endl;
                return false;
            }

            return true;
        }

        bool stop(void)
        {
            if (m_pThread.use_count() == 0)
                return true;

            m_pSdp->stop();
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
            m_pThread.reset();

            m_ios.reset();

            return true;
        }

        SdpPointer                  sdp(void)           { return m_pSdp; }
    };

}

#endif // __AIRSDP_SERVER_H__
