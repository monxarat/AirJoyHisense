/*
* AirJoyNsdDiscoveryHandler.hpp
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

#ifndef __AIRJOY_NSD_DISCOVERY_HANDLER_H__
#define __AIRJOY_NSD_DISCOVERY_HANDLER_H__

#include "SdpDiscoveryHandler.hpp"

namespace airjoy
{
    
    class AirJoyNsdDiscoveryListener;

    class AirJoyNsdDiscoveryHandler : public SdpDiscoveryHandler
    {
    private:
        AirJoyNsdDiscoveryListener * m_listener;

    public:
        AirJoyNsdDiscoveryHandler(AirJoyNsdDiscoveryListener *listener);
        virtual ~AirJoyNsdDiscoveryHandler();

        virtual void onServiceLost(SdpServiceInfoPointer serviceInfo);
        
        virtual void onServiceFound(SdpServiceInfoPointer serviceInfo);

    };

}



#endif // __AIRJOY_NSD_DISCOVERY_HANDLER_H__
