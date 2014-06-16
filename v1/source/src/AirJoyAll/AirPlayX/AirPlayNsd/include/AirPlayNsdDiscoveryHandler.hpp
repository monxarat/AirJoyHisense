/*
* AirPlayNsdDiscoveryHandler.hpp
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

#ifndef __AIRPLAY_NSD_DISCOVERY_HANDLER_H__
#define __AIRPLAY_NSD_DISCOVERY_HANDLER_H__

#include "NsdDiscoveryHandler.hpp"
#include "AirPlayServiceInfo.hpp"

namespace airjoy
{
    
    class AirPlayNsdDiscoveryListener;

    class AirPlayNsdDiscoveryHandler : public NsdDiscoveryHandler
    {
    private:
        AirPlayNsdDiscoveryListener * m_listener;

    public:
        AirPlayNsdDiscoveryHandler(AirPlayNsdDiscoveryListener *listener);
        virtual ~AirPlayNsdDiscoveryHandler();

        virtual void willStart(Nsd * nsd);
        virtual void didStart(Nsd * nsd);
        virtual void didNotStart(Nsd * nsd);

        virtual void didAddService(NsdServiceInfoPointer serviceInfo);
        virtual void didRemoveService(NsdServiceInfoPointer serviceInfo);
        virtual void didResolvService(NsdServiceInfoPointer serviceInfo);

        virtual void didGetIPAddressForService(NsdServiceInfoPointer serviceInfo);
    };

}


#endif // __AIRPLAY_NSD_DISCOVERY_HANDLER_H__
