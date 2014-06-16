/*
* AirJoyServerCore.hpp
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

#ifndef __AIRJOY_SERVER_CORE_H__
#define __AIRJOY_SERVER_CORE_H__

#include "TheBoost.hpp"
#include "AirJoyTypeDef.hpp"
#include "Worker.hpp"


namespace airjoy
{

    class UmpServer;
    class AirJoyServerHandler;

    class AirJoyServerCore
    {
    public:
        typedef boost::shared_ptr<UmpServer>                    UmpServerPointer;
        typedef boost::shared_ptr<boost::thread>                ThreadPointer;
        typedef boost::shared_ptr<AirJoyServerHandler>          AirJoyServerHandlerPointer;
		typedef boost::asio::deadline_timer						DeadlineTimer;

    private:
        bool                            m_isStart;
        boost::asio::io_service         m_ios;
        ThreadPointer                   m_pThread;
        UmpServerPointer                m_pServer;
        AirJoyServerHandlerPointer      m_pServerHandler;
		DeadlineTimer					m_timer;

    public:
        AirJoyServerCore();
        virtual ~AirJoyServerCore();

        bool start(void);
        bool stop(void);
        bool run(void);
        bool isStart(void);

        UmpServerPointer server(void);
        AirJoyServerHandlerPointer handler(void);

	private:
		void didTimeout(const boost::system::error_code& error);
    };

}

#endif // __AIRJOY_SERVER_CORE_H__

