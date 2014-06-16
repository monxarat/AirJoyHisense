
#ifndef __AJPUBLISHLISTENER_H__
#define __AJPUBLISHLISTENER_H__


#include "AirJoyNsdPublishListener.hpp"


using namespace airjoy;

class AJPublishListener: public AirJoyNsdPublishListener
{
	public:
		 /**
         * 发布服务成功
         *  @serviceInfo    [in] 服务信息
         */
        virtual void onServicePublished(AirJoyServiceInfo & serviceInfo);

        /**
         * 发布服务失败
         *  @serviceInfo    [in] 服务信息
         *  @errorCode      [in] 错误代码
         */
        virtual void onPublishFailed(AirJoyServiceInfo & serviceInfo, int errorCode);

        /**
         * 取消发布服务：成功
         *  @serviceInfo    [in] 服务信息
         *  @errorCode      [in] 错误代码
         */
        virtual void onServiceUnPublished(AirJoyServiceInfo & serviceInfo);

        /**
         * 取消发布服务：失败
         *  @serviceInfo    [in] 服务信息
         *  @errorCode      [in] 错误代码
         */
        virtual void onUnPulishFailed(AirJoyServiceInfo & serviceInfo, int errorCode);

};

#endif
