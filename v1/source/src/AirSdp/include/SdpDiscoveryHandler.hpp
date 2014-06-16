/*
* SdpDiscoveryHandler.hpp
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

#ifndef __SDP_DISCOVERY_HANDLER_H__
#define __SDP_DISCOVERY_HANDLER_H__

#include "TheBoost.hpp"
#include <string>
#include "SdpServiceInfo.hpp"

namespace airjoy
{

    class SdpDiscoveryHandler
    {
    public:
        SdpDiscoveryHandler() {}
        virtual ~SdpDiscoveryHandler() {}

        virtual void onServiceLost(SdpServiceInfoPointer serviceInfo) {}
        virtual void onServiceFound(SdpServiceInfoPointer serviceInfo) {}
    };

}


#endif // __SDP_DISCOVERY_HANDLER_H__
