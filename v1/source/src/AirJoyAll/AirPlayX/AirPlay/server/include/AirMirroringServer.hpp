/*
* AirMirroringServer.hpp
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

#ifndef __AIR_MIRRORING_SERVER_H__
#define __AIR_MIRRORING_SERVER_H__

#include <string>
#include "AirJoyApi.hpp"

namespace airjoy
{

    class AirMirroringServerCore;
    class AirMirroringServerListener;

    class AIRJOY_API AirMirroringServer
    {
    public:
        AirMirroringServer();
        virtual ~AirMirroringServer();

        /**
         * 启动服务端
         *  @listener       [in] 监听者
         */  
        void start(AirMirroringServerListener *listener);

        /**
         * 停止服务端
         */
        void stop(void);

    private:
        AirMirroringServerCore         * m_core;
        bool                       m_isStart;
    };

}

#endif // __AIR_MIRRORING_SERVER_H__
