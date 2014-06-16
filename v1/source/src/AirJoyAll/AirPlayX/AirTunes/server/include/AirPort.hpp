/*
* AirPort.hpp
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

#ifndef __AIR_PORT_H__
#define __AIR_PORT_H__

#include "Ump.hpp"

namespace airjoy
{

    class AudioBuffer;
    class AirPortDataHandler;

    class AirPort
    {
    public:
        typedef boost::shared_ptr<Ump>                          UmpPointer;
        typedef boost::shared_ptr<boost::thread>                ThreadPointer;
        typedef Ump::UmpHandlerPointer                          HandlerPointer;
        typedef boost::shared_ptr<AirPortDataHandler>           AirPortDataHandlerPointer;

    private:
        bool                            m_isStart;
        boost::asio::io_service         m_ios;
        ThreadPointer                   m_pThread;
        
        UmpPointer                      m_pData;
        UmpPointer                      m_pTiming;
        UmpPointer                      m_pControl;

        AirPortDataHandlerPointer       m_pDataHandler;
        HandlerPointer                  m_pTimingHandler;
        HandlerPointer                  m_pControlHandler;

        std::string                     m_ip;
        int                             m_clientControlPort;

    public:
        AirPort();
        virtual ~AirPort();

        bool start(void);
        bool stop(void);

        void setBuffer(AudioBuffer *audioBuffer);

        int dataPort(void);
        int timingPort(void);
        int controlPort(void);

        void setClientIp(const std::string &ip);
        void setClientControlPort(int port);

        void sendRtcpRequest(const unsigned char *buf, size_t size);
    };

}

#endif // __AIR_PORT_H__