/*
* AirJoyNsdPublishHandler.hpp
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

#ifndef __AIRJOY_NSD_PUBLISH_HANDLER_H__
#define __AIRJOY_NSD_PUBLISH_HANDLER_H__

#include "SdpPublishHandler.hpp"

namespace airjoy
{

    class AirJoyNsdPublishListener;

    class AirJoyNsdPublishHandler : public SdpPublishHandler
    {
    private:
        AirJoyNsdPublishListener   *m_listener;

    public:
        AirJoyNsdPublishHandler(AirJoyNsdPublishListener * listener);
        virtual ~AirJoyNsdPublishHandler();

        virtual void onUnPulishFailed(SdpServiceInfoPointer serviceInfo, int errorCode);

        virtual void onServiceUnPublished(SdpServiceInfoPointer serviceInfo);

        virtual void onServicePublished(SdpServiceInfoPointer serviceInfo);

        virtual void onPublishFailed(SdpServiceInfoPointer serviceInfo, int errorCode);
    };

}



#endif // __AIRJOY_NSD_PUBLISH_HANDLER_H_
