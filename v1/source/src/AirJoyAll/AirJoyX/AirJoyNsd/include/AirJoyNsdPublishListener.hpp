/*
* AirJoyNsdPublishListener.hpp
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

#ifndef __AIRJOY_NSD_PUBLISH_LISTENER_H__
#define __AIRJOY_NSD_PUBLISH_LISTENER_H__

#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"
#include "AirJoyServiceInfo.hpp"

namespace airjoy
{

    class AIRJOY_API AirJoyNsdPublishListener
    {
    public:
        AirJoyNsdPublishListener() {}
        virtual ~AirJoyNsdPublishListener() {}
           
        /**
         * 发布服务成功
         *  @serviceInfo    [in] 服务信息
         */
        virtual void onServicePublished(AirJoyServiceInfo & serviceInfo) = 0;

        /**
         * 发布服务失败
         *  @serviceInfo    [in] 服务信息
         *  @errorCode      [in] 错误代码
         */
        virtual void onPublishFailed(AirJoyServiceInfo & serviceInfo, int errorCode) = 0;

        /**
         * 取消发布服务：成功
         *  @serviceInfo    [in] 服务信息
         *  @errorCode      [in] 错误代码
         */
        virtual void onServiceUnPublished(AirJoyServiceInfo & serviceInfo) = 0;

        /**
         * 取消发布服务：失败
         *  @serviceInfo    [in] 服务信息
         *  @errorCode      [in] 错误代码
         */
        virtual void onUnPulishFailed(AirJoyServiceInfo & serviceInfo, int errorCode) = 0;
    };

}



#endif // __AIRJOY_NSD_PUBLISH_LISTENER_H__

