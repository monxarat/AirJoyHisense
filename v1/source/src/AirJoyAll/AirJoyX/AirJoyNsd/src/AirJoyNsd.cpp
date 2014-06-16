/*
 * AirJoyNsd.cpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-15
 *
 *
 */

#include "AirJoyNsd.hpp"
#include "AirJoyNsdCore.hpp"
#include "UmpServer.hpp"
#include "Sdp.hpp"
#include "AirJoyDefault.hpp"
#include "AirJoyNsdDiscoveryHandler.hpp"

using namespace airjoy;

AirJoyNsd::AirJoyNsd()
{
    m_core = new AirJoyNsdCore;
}

AirJoyNsd::~AirJoyNsd()
{
    delete m_core;
}

void AirJoyNsd::start()
{
    m_core->start();
}

void AirJoyNsd::stop()
{
    m_core->stop();
}

bool AirJoyNsd::discoveryService(AirJoyNsdDiscoveryListener *listener)
{
    std::string serviceType(AIRJOY_SERVICE_TYPE);

    Sdp::SdpDiscoveryHandlerPointer discoveryHandler = boost::make_shared<AirJoyNsdDiscoveryHandler>(listener);

    return m_core->sdp()->discoveryService(serviceType, discoveryHandler);
}

bool AirJoyNsd::stopServiceDiscovery(void)
{
    return m_core->sdp()->stop();
}

bool AirJoyNsd::publishService(AirJoyServiceInfo &serviceInfo)
{
    SdpServiceInfoPointer pService = boost::make_shared<SdpServiceInfo>();
    
    pService->setDeviceId(serviceInfo.deviceId());
    pService->setDeviceName(serviceInfo.deviceName());
    pService->setDeviceType(serviceInfo.deviceType());
    
    pService->setPort(serviceInfo.port());
    pService->setNotifyTimeout(AIRJOY_SERVICE_NOTIFY_TIMEOUT);
    pService->setPort(serviceInfo.port());

    pService->setServiceType(serviceInfo.type());
    pService->setServicePort(serviceInfo.port());
    pService->setServiceVersion(serviceInfo.version());

    return m_core->sdp()->publishService(pService);
}
