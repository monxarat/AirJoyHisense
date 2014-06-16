/*
* AirMirroringServerCore.hpp
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

#ifndef __AIR_MIRRORING_SERVER_CORE_H__
#define __AIR_MIRRORING_SERVER_CORE_H__

#include "TheBoost.hpp"
#include "AirJoyTypeDef.hpp"
#include "TcpServer.hpp"
#include "Worker.hpp"

namespace airjoy
{

    class UmpServer;

    class AirMirroringServerCore
    {
    public:
        typedef boost::shared_ptr<UmpServer>                    UmpServerPointer;
        typedef boost::shared_ptr<boost::thread>                ThreadPointer;
    
    private:
        bool                            m_isStart;
        boost::asio::io_service         m_ios;
        ThreadPointer                   m_pThread;
        UmpServerPointer                m_pUmpServer;

    public:
        AirMirroringServerCore();
        virtual ~AirMirroringServerCore();

        bool start(void);
        bool stop(void);
        bool run(void);

        void send(int sessionId, const std::string &message);

        UmpServerPointer    server(void)            { return m_pUmpServer; }
    };

}

#endif // __AIR_MIRRORING_SERVER_CORE_H__
