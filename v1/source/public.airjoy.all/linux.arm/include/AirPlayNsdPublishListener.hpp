/*
* AirPlayNsdPublishListener.hpp
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

#ifndef __AIRPLAY_NSD_PUBLISH_LISTENER_H__
#define __AIRPLAY_NSD_PUBLISH_LISTENER_H__

#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"

#include "AirPlayServiceInfo.hpp"

namespace airjoy
{
    
    /**
     * AirPlay网络服务发布监听者
     */

    class AIRJOY_API AirPlayNsdPublishListener
    {
    public:
        AirPlayNsdPublishListener() {}
        virtual ~AirPlayNsdPublishListener() {}

        /**
         * 发布服务：成功
         *  @serviceType    [in] 服务类型
         *  @serviceName    [in] 服务名称
         */
        virtual void onServicePublished(const std::string & servicetype, const std::string & serviceName) = 0;

        /**
         * 发布服务：失败
         *  @serviceType    [in] 服务类型
         *  @serviceName    [in] 服务名称
         *  @errorCode      [in] 错误代码
         */
        virtual void onPublishFailed(const std::string & servicetype, const std::string & serviceName, int errorCode) = 0;

        /**
         * 取消发布服务：成功
         *  @serviceType    [in] 服务类型
         *  @serviceName    [in] 服务名称
         *  @errorCode      [in] 错误代码
         */
        virtual void onServiceUnPublished(const std::string & servicetype, const std::string & serviceName) = 0;
        
        /**
         * 取消发布服务：失败
         *  @serviceType    [in] 服务类型
         *  @serviceName    [in] 服务名称
         *  @errorCode      [in] 错误代码
         */
        virtual void onUnPulishFailed(const std::string & servicetype, const std::string & serviceName, int errorCode) = 0;
    };

}



#endif // __AIRPLAY_NSD_PUBLISH_LISTENER_H__

