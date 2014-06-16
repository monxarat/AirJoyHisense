/*
* NsdDiscoveryHandler.hpp
*
* Service Discovery Protocol Delegate
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

#ifndef __NSD_DISCOVERY_HANDLER_H__
#define __NSD_DISCOVERY_HANDLER_H__

#include "TheBoost.hpp"

namespace airjoy
{

    class Nsd;
    class NsdServiceInfo;

    class NsdDiscoveryHandler
    {
    public:
        typedef boost::shared_ptr<NsdServiceInfo>   NsdServiceInfoPointer;

    public:
        NsdDiscoveryHandler() {}
        ~NsdDiscoveryHandler() {}

        virtual void willStart(Nsd * nsd) = 0;
        virtual void didStart(Nsd * nsd) = 0;
        virtual void didNotStart(Nsd * nsd) = 0;

        virtual void didAddService(NsdServiceInfoPointer serviceInfo) = 0;
        virtual void didRemoveService(NsdServiceInfoPointer serviceInfo) = 0;
        virtual void didResolvService(NsdServiceInfoPointer serviceInfo) = 0;

        virtual void didGetIPAddressForService(NsdServiceInfoPointer serviceInfo) = 0;
    };

}

#endif // __NSD_DISCOVERY_HANDLER_H__
