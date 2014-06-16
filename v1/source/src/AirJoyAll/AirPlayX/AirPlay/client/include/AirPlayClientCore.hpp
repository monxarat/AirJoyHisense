/*
* AirPlayClientCore.hpp
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

#ifndef __AIRPLAY_CLIENT_CORE_H__
#define __AIRPLAY_CLIENT_CORE_H__

#include "TheBoost.hpp"
#include "AirJoyTypeDef.hpp"

namespace airjoy
{

    class AnySS;
    class UmpServer;
    class UmpClient;

    class Sdp;

#ifdef __APPLE_BONJOUR__
    class Nsd;
#endif

    class AirPlayClientCore
    {
    public:
        typedef boost::shared_ptr<AnySS>                        AnySSPointer;
        typedef boost::shared_ptr<UmpServer>                    UmpServerPointer;
        typedef boost::shared_ptr<UmpClient>                    UmpClientPointer;
        typedef boost::shared_ptr<Sdp>                          SdpPointer;
        typedef boost::shared_ptr<boost::thread>                ThreadPointer;
    
    #ifdef __APPLE_BONJOUR__
        typedef boost::shared_ptr<Nsd>                          NsdPointer;
    #endif

    private:
        bool                            m_isStart;
        boost::asio::io_service         m_ios;
        ThreadPointer                   m_pThread;
        AnySSPointer                    m_pSS;
        SdpPointer                      m_pSdp;
        UmpServerPointer                m_pUmpServer;
        UmpClientPointer                m_pUmpClient;
        UmpServerPointer                m_pAirPlayServer;
        UmpClientPointer                m_pAirPlayClient;

    #ifdef __APPLE_BONJOUR__
        NsdPointer                      m_pAirPlayNsd;
    #endif

    public:
        AirPlayClientCore();
        virtual ~AirPlayClientCore();

        // start & stop anyplay
        bool start(void);
        bool stop(void);
        bool run(void);

        AnySSPointer        ss(void)                { return m_pSS; }
        SdpPointer          sdp(void)               { return m_pSdp; }
        UmpServerPointer    server(void)            { return m_pUmpServer; }
        UmpClientPointer    client(void)            { return m_pUmpClient; }
        UmpServerPointer    airplayServer(void)     { return m_pAirPlayServer; }
        UmpClientPointer    airplayClient(void)     { return m_pAirPlayClient; }

    #ifdef __APPLE_BONJOUR__
        NsdPointer          airplayNsd(void)        { return m_pAirPlayNsd; }
    #endif
    };

}

#endif // __AIRPLAY_CLIENT_CORE_H__
