/*
* AirJoyClientCore.hpp
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

#ifndef __AIRJOY_CLIENT_CORE_H__
#define __AIRJOY_CLIENT_CORE_H__


#include "TheBoost.hpp"
#include "AirJoyTypeDef.hpp"

namespace airjoy
{

    class UmpClient;

    class AirJoyClientCore
    {
    public:
        typedef boost::shared_ptr<UmpClient>                    UmpClientPointer;
        typedef boost::shared_ptr<boost::thread>                ThreadPointer;
		typedef boost::asio::deadline_timer						DeadlineTimer;

    private:
        bool                            m_isStart;
        boost::asio::io_service         m_ios;
        ThreadPointer                   m_pThread;
        UmpClientPointer                m_pClient;
		DeadlineTimer					m_timer;

    public:
        AirJoyClientCore();
        virtual ~AirJoyClientCore();

        bool start(void);
        bool stop(void);
        bool run(void);

        UmpClientPointer    client(void)        { return m_pClient; }

	private:
		void didTimeout(const boost::system::error_code& error);
    };

}


#endif // __AIRJOY_CLIENT_CORE_H__