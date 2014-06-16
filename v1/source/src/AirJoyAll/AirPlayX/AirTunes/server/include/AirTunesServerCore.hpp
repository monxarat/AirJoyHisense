/*
* AirTunesServerCore.hpp
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

#ifndef __AIRTUNES_SERVER_CORE_H__
#define __AIRTUNES_SERVER_CORE_H__

#include "TheBoost.hpp"
#include "AirJoyTypeDef.hpp"
#include "TcpServer.hpp"
#include "Worker.hpp"

namespace airjoy
{

    class UmpServer;

    class AirTunesServerCore
    {
    public:
        typedef boost::shared_ptr<UmpServer>                    UmpServerPointer;
        typedef boost::shared_ptr<boost::thread>                ThreadPointer;
		typedef boost::asio::deadline_timer						DeadlineTimer;

    private:
        bool                            m_isStart;
        boost::asio::io_service         m_ios;
        ThreadPointer                   m_pThread;
        UmpServerPointer                m_pUmpServer;
		DeadlineTimer					m_timer;

    public:
        AirTunesServerCore();
        virtual ~AirTunesServerCore();

        bool start(void);
        bool stop(void);
        bool run(void);

        UmpServerPointer    server(void)            { return m_pUmpServer; }
    
        boost::asio::io_service & getIos(void)      { return m_ios; }
	
    private:
		void didTimeout(const boost::system::error_code& error);
    };

}

#endif // __AIRTUNES_SERVER_CORE_H__
