/*
* SdpPublishHandler.cpp
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

#include "SdpPublishWorker.hpp"
#include "HttpMessage.hpp"
#include "SdpMaster.hpp"
#include "SdpServiceInfo.hpp"

using namespace airjoy;

SdpPublishWorker::SdpPublishWorker()
    : m_needPublish(false)
{
}

SdpPublishWorker::~SdpPublishWorker()
{
}

bool SdpPublishWorker::publishService(SdpServiceInfoPointer service)
{
    m_needPublish = true;
    m_serviceInfo = service;
    
    return true;
}
        
bool SdpPublishWorker::unPublishService(SdpServiceInfoPointer service)
{
    m_needPublish = false;

    return true;
}

uint64_t SdpPublishWorker::getPublishTimeout(SdpMaster *master)
{
    if (m_needPublish)
        return m_serviceInfo->notifyTimeout();

    return 30;
}

void SdpPublishWorker::onTimeout(SdpMaster *master)
{
    if (! m_needPublish)
        return;

    std::string timeout;
    std::string servicePort;
    std::string deviceType;

    try 
    {
        timeout = boost::lexical_cast<std::string>(m_serviceInfo->notifyTimeout());
        servicePort = boost::lexical_cast<std::string>(m_serviceInfo->servicePort());
        deviceType = boost::lexical_cast<std::string>(m_serviceInfo->deviceType());
    }
    catch (boost::bad_lexical_cast&)
    {
        std::cout << "SdpPublishWorker::onTimeout error!" << std::endl;
    }

    // 发送服务在线宣告
    HttpMessage httpRequest;
    httpRequest.setHttpType(HttpRequest);
    httpRequest.setHttpVersionMajor(1);
    httpRequest.setHttpVersionMinor(1);
    httpRequest.setHttpMethod("NOTIFY");
    httpRequest.setUri("*");

    httpRequest.addHeaders("HOST", "239.255.255.250:1900");
    httpRequest.addHeaders("MAN", "airjoy:sdp/1.0");
    httpRequest.addHeaders("NTS", "sdp:alive");
    httpRequest.addHeaders("TIMEOUT", timeout);
    httpRequest.addHeaders("SERVICE-TYPE", m_serviceInfo->serviceType());
    httpRequest.addHeaders("SERVICE-PORT", servicePort);
    httpRequest.addHeaders("SERVICE-VER", m_serviceInfo->serviceVersion());
    httpRequest.addHeaders("DEVICE-ID", m_serviceInfo->deviceId());
    httpRequest.addHeaders("DEVICE-TYPE", deviceType);
    httpRequest.addHeaders("DEVICE-NAME", m_serviceInfo->deviceName());

    master->sendToAll(httpRequest.toString());
}