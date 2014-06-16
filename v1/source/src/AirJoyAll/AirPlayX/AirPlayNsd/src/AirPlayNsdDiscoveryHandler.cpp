/*
* AirPlayNsdDiscoveryHandler.cpp
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

#include "AirPlayNsdDiscoveryHandler.hpp"
#include "AirPlayNsdDiscoveryListener.hpp"
#include "Nsd.hpp"
#include "AirPlayServiceInfo.hpp"
#include "NsdServiceInfo.hpp"

using namespace airjoy;


AirPlayNsdDiscoveryHandler::AirPlayNsdDiscoveryHandler(AirPlayNsdDiscoveryListener *listener)
    : m_listener(listener)
{
}

AirPlayNsdDiscoveryHandler::~AirPlayNsdDiscoveryHandler() 
{
}

void AirPlayNsdDiscoveryHandler::willStart(Nsd * nsd)
{
}
 
void AirPlayNsdDiscoveryHandler::didStart(Nsd * nsd)
{
    if (m_listener)
        m_listener->onDiscoveryStarted(nsd->discoveryServiceType());
}

void AirPlayNsdDiscoveryHandler::didNotStart(Nsd * nsd)
{
    if (m_listener)
        m_listener->onStartDiscoveryFailed(nsd->discoveryServiceType(), 0);
}

void AirPlayNsdDiscoveryHandler::didAddService(NsdServiceInfoPointer serviceInfo)
{
}

void AirPlayNsdDiscoveryHandler::didRemoveService(NsdServiceInfoPointer serviceInfo)
{
    if (m_listener)
    {
        AirPlayServiceInfo service;
        service.setDeviceId(serviceInfo->getTXTRecordValueWithKey("deviceid"));
        service.setDeviceModel(serviceInfo->getTXTRecordValueWithKey("model"));
        service.setDeviceName(serviceInfo->getName());
        service.setPort(serviceInfo->getPort());
        service.setIp(serviceInfo->getIPWithString());

        m_listener->onServiceLost(serviceInfo->getType(),
                                  serviceInfo->getName(),
                                  serviceInfo->getIPWithString(),
                                  serviceInfo->getPort());
    }
}

void AirPlayNsdDiscoveryHandler::didResolvService(NsdServiceInfoPointer serviceInfo)
{
}

void AirPlayNsdDiscoveryHandler::didGetIPAddressForService(NsdServiceInfoPointer serviceInfo)
{
    if (m_listener)
    {
        AirPlayServiceInfo service;
        service.setDeviceId(serviceInfo->getTXTRecordValueWithKey("deviceid"));
        service.setDeviceModel(serviceInfo->getTXTRecordValueWithKey("model"));
        service.setDeviceName(serviceInfo->getName());
        service.setPort(serviceInfo->getPort());
        service.setIp(serviceInfo->getIPWithString());
        
        //std::cout << "features: " << serviceInfo->getTXTRecordValueWithKey("features") << std::endl;
        //std::cout << "flags: " << serviceInfo->getTXTRecordValueWithKey("flags") << std::endl;
        //std::cout << "vv: " << serviceInfo->getTXTRecordValueWithKey("vv") << std::endl;

        m_listener->onServiceFound(serviceInfo->getType(),
                                   serviceInfo->getName(),
                                   serviceInfo->getIPWithString(),
                                   serviceInfo->getPort());
    }
}