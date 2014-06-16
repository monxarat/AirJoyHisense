/*
* AirPlayNsdCore.hpp
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

#ifndef __AIRPLAY_NSD_CORE_H__
#define __AIRPLAY_NSD_CORE_H__

#include "TheBoost.hpp"
#include "AirJoyTypeDef.hpp"
#include "TcpServer.hpp"
#include "Worker.hpp"
#include "Nsd.hpp"

namespace airjoy
{

    class AirPlayNsdCore
    {
    public:
        typedef boost::shared_ptr<Nsd>                          NsdPointer;
        typedef boost::shared_ptr<boost::thread>                ThreadPointer;
		typedef boost::asio::deadline_timer						DeadlineTimer;
        typedef Nsd::NsdServiceInfoPointer                      ServicePointer;

    private:
        bool                            m_isStart;
        boost::asio::io_service         m_ios;
        ThreadPointer                   m_pThread;
        NsdPointer                      m_pNsd;
		DeadlineTimer					m_timer;

        std::list<ServicePointer>       m_services;

    public:
        AirPlayNsdCore();
        virtual ~AirPlayNsdCore();

        bool start(void);
        bool stop(void);
        bool run(void);

        NsdPointer  nsd(void)        { return m_pNsd; }

        bool addService(ServicePointer s) ;
        bool removeService(ServicePointer s);
        bool removeAllService(void);

	private:
		void didTimeout(const boost::system::error_code& error);
    };

}

#endif // __AIRPLAY_NSD_CORE_H__
