/*
* AirJoyNsdPublishHandler.cpp
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


#include "AirJoyNsdPublishHandler.hpp"
#include "AirJoyNsdPublishListener.hpp"
#include "Sdp.hpp"

using namespace airjoy;

AirJoyNsdPublishHandler::AirJoyNsdPublishHandler(AirJoyNsdPublishListener * listener)
    : m_listener(listener)
{
}

AirJoyNsdPublishHandler::~AirJoyNsdPublishHandler() 
{
}

void AirJoyNsdPublishHandler::onUnPulishFailed(SdpServiceInfoPointer serviceInfo, int errorCode) 
{
    if (m_listener)
    {
        AirJoyServiceInfo service;
        service.setDeviceId(serviceInfo->deviceId());
        service.setDeviceName(serviceInfo->deviceName());
        service.setDeviceType(serviceInfo->deviceType());
        service.setPort(serviceInfo->servicePort());

        m_listener->onUnPulishFailed(service, errorCode);
    }
}

void AirJoyNsdPublishHandler::onServiceUnPublished(SdpServiceInfoPointer serviceInfo) 
{
    if (m_listener)
    {
        AirJoyServiceInfo service;
        service.setDeviceId(serviceInfo->deviceId());
        service.setDeviceName(serviceInfo->deviceName());
        service.setDeviceType(serviceInfo->deviceType());
        service.setPort(serviceInfo->servicePort());

        m_listener->onServiceUnPublished(service);    
    }
}

void AirJoyNsdPublishHandler::onServicePublished(SdpServiceInfoPointer serviceInfo) 
{
    if (m_listener)
    {
        AirJoyServiceInfo service;
        service.setDeviceId(serviceInfo->deviceId());
        service.setDeviceName(serviceInfo->deviceName());
        service.setDeviceType(serviceInfo->deviceType());
        service.setPort(serviceInfo->servicePort());

        m_listener->onServicePublished(service);
    }
}

void AirJoyNsdPublishHandler::onPublishFailed(SdpServiceInfoPointer serviceInfo, int errorCode) 
{
    if (m_listener)
    {
        AirJoyServiceInfo service;
        service.setDeviceId(serviceInfo->deviceId());
        service.setDeviceName(serviceInfo->deviceName());
        service.setDeviceType(serviceInfo->deviceType());
        service.setPort(serviceInfo->servicePort());

        m_listener->onPublishFailed(service, errorCode);
    }
}
