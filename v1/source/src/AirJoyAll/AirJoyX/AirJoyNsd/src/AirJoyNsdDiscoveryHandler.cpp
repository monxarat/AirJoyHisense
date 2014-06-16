/*
* AirJoyNsdDiscoveryHandler.cpp
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

#include "AirJoyNsdDiscoveryHandler.hpp"
#include "AirJoyNsdDiscoveryListener.hpp"
#include "Sdp.hpp"

using namespace airjoy;


AirJoyNsdDiscoveryHandler::AirJoyNsdDiscoveryHandler(AirJoyNsdDiscoveryListener *listener) 
    : m_listener(listener)
{
}

AirJoyNsdDiscoveryHandler::~AirJoyNsdDiscoveryHandler() 
{
}
        
void AirJoyNsdDiscoveryHandler::onServiceLost(SdpServiceInfoPointer serviceInfo)
{
    if (m_listener)
    {
        AirJoyServiceInfo service;
        service.setDeviceId(serviceInfo->deviceId());
        service.setDeviceName(serviceInfo->deviceName());
        service.setDeviceType(serviceInfo->deviceType());
        service.setIp(serviceInfo->ip());
		service.setPort(serviceInfo->servicePort());

        m_listener->onServiceLost(service);
    }
}

void AirJoyNsdDiscoveryHandler::onServiceFound(SdpServiceInfoPointer serviceInfo) 
{
    if (m_listener)
    {
        AirJoyServiceInfo service;
        service.setDeviceId(serviceInfo->deviceId());
        service.setDeviceName(serviceInfo->deviceName());
        service.setDeviceType(serviceInfo->deviceType());
        service.setIp(serviceInfo->ip());
		service.setPort(serviceInfo->servicePort());

        m_listener->onServiceFound(service);
    }
}