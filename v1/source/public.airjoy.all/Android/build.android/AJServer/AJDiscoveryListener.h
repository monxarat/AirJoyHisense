
#ifndef __AJDISCOVERYLISTENER_H__
#define __AJDISCOVERYLISTENER_H__


#include "AirJoyNsdDiscoveryListener.hpp"


using namespace airjoy;

class AJDiscoveryListener: public AirJoyNsdDiscoveryListener 
{
	public:
#if 0
		 /**
         * 启动监听：成功
         *  @serviceType    [in] 服务类型
         */
        virtual void onDiscoveryStarted(const std::string serviceType);
        
        /**
         * 启动监听：失败
         *  @serviceType    [in] 服务类型
         *  @errorCode      [in] 错误代码
         */
        virtual void onStartDiscoveryFailed(const std::string serviceType, int errorCode);

        /**
         * 停止监听：成功
         *  @serviceType    [in] 服务类型
         */
        virtual void onDiscoveryStopped(const std::string serviceType);

        /**
         * 停止监听：失败
         *  @serviceType    [in] 服务类型
         *  @errorCode      [in] 错误代码
         */
        virtual void onStopDiscoveryFailed(const std::string serviceType, int errorCode);
#endif
        
        /**
         * 某个服务已经下线
         *  @serviceInfo    [in] 服务信息
         */
         virtual void onServiceLost(AirJoyServiceInfo &serviceInfo);

        /**
         * 某个服务已经上线
         *  @serviceInfo    [in] 服务信息
         */
        virtual void onServiceFound(AirJoyServiceInfo &serviceInfo);

};

#endif
