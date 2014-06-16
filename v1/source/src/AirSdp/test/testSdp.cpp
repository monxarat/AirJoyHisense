#include <string>
#include <iostream>

#include "SdpPublishHandler.hpp"
#include "SdpDiscoveryHandler.hpp"

#include "AirSdpServer.hpp"

using namespace airjoy;

class PublishHandler : public SdpPublishHandler
{
public:

    PublishHandler() {}
    virtual ~PublishHandler() {}

    virtual void onUnPulishFailed(SdpServiceInfoPointer serviceInfo, int errorCode) 
    {
        std::cout << "onUnPulishFailed" << std::endl;
    }

    virtual void onServiceUnPublished(SdpServiceInfoPointer serviceInfo) 
    {
        std::cout << "onServiceUnPublished" << std::endl;
    }

    virtual void onServicePublished(SdpServiceInfoPointer serviceInfo) 
    {
        std::cout << "onServicePublished" << std::endl;
    }

    virtual void onPublishFailed(SdpServiceInfoPointer serviceInfo, int errorCode) 
    {
        std::cout << "onPublishFailed" << std::endl;
    }
};

class DiscoveryHandler : public SdpDiscoveryHandler
{
public:

    DiscoveryHandler() {}
    virtual ~DiscoveryHandler() {}

    virtual void onStopDiscoveryFailed(const std::string serviceType, int errorCode) 
    {
        std::cout << "onStopDiscoveryFailed" << std::endl;
    }

    virtual void onStartDiscoveryFailed(const std::string serviceType, int errorCode) 
    {
        std::cout << "onStartDiscoveryFailed" << std::endl;
    }

    virtual void onServiceLost(SdpServiceInfoPointer serviceInfo) 
    {
         std::cout << "onServiceLost" << std::endl;
    }

    virtual void onServiceFound(SdpServiceInfoPointer serviceInfo) 
    {
        std::cout << "onServiceFound : " << serviceInfo->deviceName() << std::endl;
    }

    virtual void onDiscoveryStopped(const std::string serviceType) 
    {
         std::cout << "onDiscoveryStopped" << std::endl;
    }

    virtual void onDiscoveryStarted(const std::string serviceType) 
    {
         std::cout << "onDiscoveryStarted" << std::endl;
    }
};

AirSdpServer gSdp;
bool gLoop = true;
    
SdpServiceInfoPointer serviceInfo = boost::make_shared<SdpServiceInfo>();

void  parseCommand(const std::string &cmd)
{
    if (cmd == "help" || cmd == "h")
    {
        std::cout << "---------- help ----------" << std::endl;
        std::cout << "help(h) -- help infomation" << std::endl;
        std::cout << "exit(x) -- exit" << std::endl;
        return;
    }

    if (cmd == "exit" || cmd == "x")
    {
        gSdp.stop();
        gLoop = false;
        return;
    }
}

void command()
{
    std::string buf;

    while (gLoop)
    {
        std::cout << "> ";
        buf.clear();
        std::cin >> buf;
        parseCommand(buf);
    }
}

int main()
{
    gSdp.start();

    Sdp::SdpDiscoveryHandlerPointer pDiscoveryHandler = boost::make_shared<DiscoveryHandler>();
    gSdp.sdp()->discoveryService("airjoy:ump/1.0", pDiscoveryHandler);
    
    serviceInfo->setDeviceId("Linux");
    serviceInfo->setDeviceName("Linux");
    serviceInfo->setDeviceType(DeviceTypePc);
    serviceInfo->setPort(1900);
    serviceInfo->setNotifyTimeout(30);
    serviceInfo->setServiceType("airjoy:ump/1.0");
    serviceInfo->setServicePort(8000);
    serviceInfo->setServiceVersion("1.0");

    Sdp::SdpPublishHandlerPointer pPublishHandler = boost::make_shared<PublishHandler>();
    gSdp.sdp()->publishService(serviceInfo, pPublishHandler);

    command();

    return 0;
}
