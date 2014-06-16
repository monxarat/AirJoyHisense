/*
 * AirPlayNsd.hpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-15
 *
 *
 */

#ifndef __AIRPLAY_NSD_H__
#define __AIRPLAY_NSD_H__

#include <string>
#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"

#include "AirPlayServiceInfo.hpp"
#include "AirTunesServiceInfo.hpp"

#include "AirPlayNsdDiscoveryListener.hpp"
#include "AirPlayNsdPublishListener.hpp"

namespace airjoy
{

    class AirPlayNsdCore;

    class AIRJOY_API AirPlayNsd
    {
    public:
        AirPlayNsd();
        virtual ~AirPlayNsd();
        
        /**
         * 启动自动发现服务
		 * @serviceType       [in] 服务类型
         * @listener          [in] 监听者
         * @return
         *      true  - 启动成功
         *      false - 启动失败
         */
        bool discoveryService(const std::string &serviceType, AirPlayNsdDiscoveryListener *listener);
        
        /**
         * 停止自动发现服务
         */    
        bool stopServiceDiscovery(void);
   
        /**
         * 发布服务
         *  @serviceInfo    [in] AirPlay服务端信息
         *  @listener       [in] 监听者
         *  @return
         *      true  - 发布成功
         *      false - 发布失败
         */
        bool publishService(AirPlayServiceInfo &serviceInfo, AirPlayNsdPublishListener *listener);
        
		/**
         * 发布服务
         *  @serviceInfo    [in] AirTunes服务端信息
         *  @listener       [in] 监听者
         *  @return
         *      true  - 发布成功
         *      false - 发布失败
         */
        bool publishService(AirTunesServiceInfo &serviceInfo, AirPlayNsdPublishListener *listener);

    private:
        AirPlayNsdCore     * m_core;
        bool                 m_isStart;
    };

}

#endif // __AIRPLAY_NSD_H__
