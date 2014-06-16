/*
* AirJoyNsdCore.hpp
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

#ifndef __AIRJOY_NSD_CORE_H__
#define __AIRJOY_NSD_CORE_H__

#include "TheBoost.hpp"
#include "AirJoyTypeDef.hpp"

namespace airjoy
{

    class Sdp;

    class AirJoyNsdCore
    {
    public:
        typedef boost::shared_ptr<Sdp>                          SdpPointer;
        typedef boost::shared_ptr<boost::thread>                ThreadPointer;
    
    private:
        bool                            m_isStart;
        boost::asio::io_service         m_ios;
        ThreadPointer                   m_pThread;
        SdpPointer                      m_pSdp;

    public:
        AirJoyNsdCore();
        virtual ~AirJoyNsdCore();

        bool start(void);
        bool stop(void);
        bool run(void);

        SdpPointer          sdp(void)               { return m_pSdp; }
    };

}

#endif // __AIRJOY_NSD_CORE_H__
