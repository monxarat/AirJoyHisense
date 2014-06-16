/*
* AirPlayNsdDiscoveryListener.hpp
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

#ifndef __AIRPLAY_NSD_DISCOVERY_LISTENER_H__
#define __AIRPLAY_NSD_DISCOVERY_LISTENER_H__

#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"

#include "AirPlayServiceInfo.hpp"

namespace airjoy
{

    /**
     * AirPlay网络服务发现监听者
     */

    class AIRJOY_API AirPlayNsdDiscoveryListener
    {
    public:
        AirPlayNsdDiscoveryListener() {}
        virtual ~AirPlayNsdDiscoveryListener() {}
        
        /**
         * 启动自动发现服务：成功
         *  @serviceType    [in] 服务类型
         */
        virtual void onDiscoveryStarted(const std::string serviceType) = 0;

        /**
         * 启动自动发现服务：失败
         *  @serviceType    [in] 服务类型
         *  @errorCode      [in] 错误代码
         */
        virtual void onStartDiscoveryFailed(const std::string serviceType, int errorCode) = 0;
                
        /**
         * 停止自动发现服务：成功
         *  @serviceType    [in] 服务类型
         */
        virtual void onDiscoveryStopped(const std::string serviceType) = 0;
  
        /**
         * 停止自动发现服务：失败
         *  @serviceType    [in] 服务类型
         *  @errorCode      [in] 错误代码
         */
        virtual void onStopDiscoveryFailed(const std::string serviceType, int errorCode) = 0;

        /**
         * 某个服务已经下线
         *  @serviceInfo    [in] 服务信息
         */
        virtual void onServiceLost(const std::string serviceType, 
                                   const std::string serviceName, 
                                   const std::string serviceIp,
                                   uint16_t servicePort) = 0;
        
        /**
         * 某个服务已经上线
         *  @serviceInfo    [in] 服务信息
         */
        virtual void onServiceFound(const std::string serviceType,
                                    const std::string serviceName,
                                    const std::string serviceIp,
                                    uint16_t servicePort) = 0;

    };

}



#endif // __AIRPLAY_NSD_DISCOVERY_LISTENER_H__
