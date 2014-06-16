/*
* SdpPublishHandler.hpp
*
* Service Discovery Protocol
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

#ifndef __SDP_PUBLISH_HANDLER_H__
#define __SDP_PUBLISH_HANDLER_H__

#include "TheBoost.hpp"
#include "SdpServiceInfo.hpp"

namespace airjoy
{

    class SdpPublishHandler
    {
    public:
        SdpPublishHandler() {}
        virtual ~SdpPublishHandler() {}

        virtual void onServiceUnPublished(SdpServiceInfoPointer serviceInfo) {}
        virtual void onServicePublished(SdpServiceInfoPointer serviceInfo) {}

        virtual void onUnPulishFailed(SdpServiceInfoPointer serviceInfo, int errorCode) {}
        virtual void onPublishFailed(SdpServiceInfoPointer serviceInfo, int errorCode) {}
    };

}


#endif // __SDP_PUBLISH_HANDLER_H__
