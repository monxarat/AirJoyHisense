/*
* SdpDiscoveryWorker.cpp
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

#include "SdpDiscoveryWorker.hpp"
#include "SdpMaster.hpp"
#include "SdpServiceInfo.hpp"

using namespace airjoy;

#define SERVICE_DEFAULT_TIMEOUT     10
#define DISCOVER_DEFAULT_TIMEOUT    15

#if 0
#define SDP_DISCOVERY_DEBUG
#endif

SdpDiscoveryWorker::SdpDiscoveryWorker()
    : m_hasPublishedService(false)
    , m_isStartDiscover(false)
    , m_serviceTimeout(SERVICE_DEFAULT_TIMEOUT)
    , m_discoverTimeout(DISCOVER_DEFAULT_TIMEOUT)
{
}

SdpDiscoveryWorker::~SdpDiscoveryWorker()
{
}

void SdpDiscoveryWorker::setServiceOnlineTimeout(int second)
{
    m_serviceTimeout = second;
}

bool SdpDiscoveryWorker::startDiscover(const std::string & serviceType, SdpDiscoveryHandlerPointer handler)
{
    m_serviceType = serviceType;
    m_handler = handler;
    m_isStartDiscover = true;
    
    m_services.clear();

    m_clock.restart();

    return true;
}

bool SdpDiscoveryWorker::stopDiscover(void)
{
    m_isStartDiscover = false;

    return true;
}

uint64_t SdpDiscoveryWorker::getDiscoverTimeout(SdpMaster *master)
{
    //std::cout << "SdpDiscoveryWorker::getDiscoverTimeout: " << m_serviceTimeout << std::endl;

    return m_discoverTimeout;
}

void SdpDiscoveryWorker::onDiscoverTimeout(SdpMaster *master)
{
    if (m_isStartDiscover == false)
        return;

    searchService(master);
}

uint64_t SdpDiscoveryWorker::getServiceTimeout(SdpMaster *master)
{
    //std::cout << "SdpDiscoveryWorker::getServiceTimeout: " << std::endl;

    if (m_isStartDiscover == false)
        return m_serviceTimeout;

    boost::mutex::scoped_lock lock(m_servicesMutex);
    if (m_services.size() == 0)
        return m_serviceTimeout;

    // 第一个服务的超时时间，为最小超时时间

    SdpServiceInfoPointer p = m_services.front();

    uint64_t timeout = 1;
    uint64_t now = this->nowClock();
    if (p->nextNotifyTime() > now)
        timeout = p->nextNotifyTime() - now;
     
    //std::cout << "ServiceTimeout: " << timeout << std::endl;

    return timeout;
}

void SdpDiscoveryWorker::onServiceTimeout(SdpMaster *master)
{
    //std::cout << "SdpDiscoveryWorker::onServiceTimeout: " << std::endl;

    if (m_isStartDiscover == false)
        return;
	
    boost::mutex::scoped_lock lock(m_servicesMutex);
    if (m_services.size() == 0)
        return;

    // 查询第一个服务是否超时（也许服务已经收到在线宣告，已经不超时了）

    SdpServiceInfoPointer p = m_services.front();

    // 下次宣告时间已过，说明服务已经超时，需要发送查询请求
    uint64_t now = this->nowClock();

    if (p->nextNotifyTime() < now)
    {
#ifdef SDP_DISCOVERY_DEBUG
        std::cout << "[ " << now << " ] " <<"service is timeout: " << p->deviceName() << std::endl;
#endif

        if (p->retry() >= 3)
        {
            m_handler->onServiceLost(p);
            m_services.pop_front();
        }
        else
        {
            p->setNextNotifyTime(now + 1);
            p->setRetry(p->retry() + 1);

            searchService(master, p);
        }
    }
    else
    {
#ifdef SDP_DISCOVERY_DEBUG
        std::cout << "[ " << now << " ] " <<" service is not timeout: " << p->deviceName() << " nextnotifytime: " << p->nextNotifyTime() << std::endl;
#endif
    }
}

void SdpDiscoveryWorker::onReceived(SdpMaster *master, const std::string &ip, uint16_t port, const std::string &buf)
{
    HttpMessage httpMessage;
    HttpParserStatus status = httpMessage.loadBytes(buf.c_str(), buf.length());
    if (status == HttpParserDone)
    {
        // debug
        //std::cout << "------------- onReceived --> ok ------------------: " << ip << std::endl;
        //httpMessage.print();

        if (httpMessage.httpType() == HttpRequest)
        {
            if (httpMessage.httpMethod() == "M-SEARCH")
            {
                handleMsearch(master, ip, port, httpMessage);
            }

            else if (httpMessage.httpMethod() == "NOTIFY")
            {
                if (httpMessage.getValueByName("NTS") == "sdp:alive")
                    handleServiceFound(ip, port, httpMessage);

                else if (httpMessage.getValueByName("NTS") == "sdp:byebye")
                    handleServiceLost(ip, port, httpMessage);
            }
        }
        else if (httpMessage.httpType() == HttpResponse)
        {
            if (httpMessage.httpResponseCode() == 200)
            {
                handleServiceFound(ip, port, httpMessage);
            }
        }
    }
    else
    {
        //std::cout << "------------- onReceived error !!!!! ------------------: " << ip << std::endl;
        //std::cout << buf << std::endl;
    }
}

void SdpDiscoveryWorker::addPublishedService(SdpServiceInfoPointer service)
{
    m_hasPublishedService = true;
    m_publishedService = service;
}

void SdpDiscoveryWorker::removePublishedService(SdpServiceInfoPointer service)
{
    m_hasPublishedService = false;
	m_publishedService.reset();
}

uint64_t SdpDiscoveryWorker::nowClock(void)
{
    return (uint64_t)m_clock.elapsed();
}

void SdpDiscoveryWorker::searchService(SdpMaster *master)
{
#ifdef SDP_DISCOVERY_DEBUG
    std::cout << "search service : Is anyone Online ?" << std::endl;
#endif // SDP_DEBUG

    // 查询在线服务有哪些？
    HttpMessage httpRequest;
    httpRequest.setHttpType(HttpRequest);
    httpRequest.setHttpVersionMajor(1);
    httpRequest.setHttpVersionMinor(1);
    httpRequest.setHttpMethod("M-SEARCH");
    httpRequest.setUri("*");

    httpRequest.addHeaders("HOST", "239.255.255.250:1900");
    httpRequest.addHeaders("MAN", "airjoy:sdp/1.0");
    httpRequest.addHeaders("TARGET-SERVICE-TYPE", m_serviceType);

    master->sendToAll(httpRequest.toString());
}

void SdpDiscoveryWorker::searchService(SdpMaster *master, SdpServiceInfoPointer service)
{
#ifdef SDP_DISCOVERY_DEBUG
    std::cout << "search service : Are you Online ? -> " << service->deviceName() << std::endl;
#endif // SDP_DEBUG

    // 查询某服务是否还在线？
    HttpMessage httpRequest;
    httpRequest.setHttpType(HttpRequest);
    httpRequest.setHttpVersionMajor(1);
    httpRequest.setHttpVersionMinor(1);
    httpRequest.setHttpMethod("M-SEARCH");
    httpRequest.setUri("*");

    httpRequest.addHeaders("HOST", "239.255.255.250:1900");
    httpRequest.addHeaders("MAN", "airjoy:sdp/1.0");
    httpRequest.addHeaders("TARGET-SERVICE-TYPE", m_serviceType);

    master->sendTo(service->ip(), service->port(), httpRequest.toString());
}

void SdpDiscoveryWorker::handleMsearch(SdpMaster *master, const std::string &ip, uint16_t port, HttpMessage &httpMessage)
{
	if (! m_hasPublishedService)
        return;

    if (httpMessage.getValueByName("TARGET-SERVICE-TYPE") != m_publishedService->serviceType())
        return;

    std::string timeout;
    std::string servicePort;
    std::string deviceType;

    try 
    {
        timeout = boost::lexical_cast<std::string>(m_publishedService->notifyTimeout());
        servicePort = boost::lexical_cast<std::string>(m_publishedService->servicePort());
        deviceType = boost::lexical_cast<std::string>(m_publishedService->deviceType());
    }
    catch (boost::bad_lexical_cast&)
    {
        std::cout << "SdpPublishWorker::onTimeout error!" << std::endl;
    }

    // 发送应答
    HttpMessage httpResponse;
    httpResponse.setHttpType(HttpResponse);
    httpResponse.setHttpVersionMajor(1);
    httpResponse.setHttpVersionMinor(1);
    httpResponse.setHttpResponseCode(200);
    httpResponse.sethttpResponseStatus("OK");

    httpResponse.addHeaders("HOST", "239.255.255.250:1900");
    httpResponse.addHeaders("MAN", "airjoy:sdp/1.0");
    httpResponse.addHeaders("NTS", "sdp:alive");
    httpResponse.addHeaders("TIMEOUT", timeout);
    httpResponse.addHeaders("SERVICE-TYPE", m_publishedService->serviceType());
    httpResponse.addHeaders("SERVICE-PORT", servicePort);
    httpResponse.addHeaders("SERVICE-VER", m_publishedService->serviceVersion());
    httpResponse.addHeaders("DEVICE-ID", m_publishedService->deviceId());
    httpResponse.addHeaders("DEVICE-TYPE", deviceType);
    httpResponse.addHeaders("DEVICE-NAME", m_publishedService->deviceName());

    master->sendTo(ip, port, httpResponse.toString());
}

void SdpDiscoveryWorker::handleServiceFound(const std::string &ip, uint16_t port, HttpMessage &httpMessage)
{
	if (m_isStartDiscover == false)
		return;

    if (httpMessage.getValueByName("SERVICE-TYPE") != m_serviceType)
        return;

    std::string deviceId = httpMessage.getValueByName("DEVICE-ID");
    uint64_t timeout;
    int servicePort;
    int deviceType;
    
    if (m_hasPublishedService)
    {
        // self
        if (m_publishedService->deviceId() == deviceId)
            return;
    }

	try
	{
		timeout = boost::lexical_cast<int>(httpMessage.getValueByName("TIMEOUT"));
		servicePort = boost::lexical_cast<int>(httpMessage.getValueByName("SERVICE-PORT"));
		deviceType = boost::lexical_cast<int>(httpMessage.getValueByName("DEVICE-TYPE"));

        // 调整服务有效时间
        if (m_serviceTimeout < timeout)
            timeout = m_serviceTimeout;
	}
	catch (boost::bad_lexical_cast&)
	{
		std::cout << "SdpDiscoveryWorker::handleServiceFound error!" << std::endl;
	}

    uint64_t now = this->nowClock();

    // new service
    SdpServiceInfoPointer service = boost::make_shared<SdpServiceInfo>();
    service->setDeviceId(deviceId);
    service->setDeviceName(httpMessage.getValueByName("DEVICE-NAME"));
    service->setNotifyTimeout(timeout);
    service->setServicePort(servicePort);
    service->setDeviceType((DeviceType)deviceType);
    service->setServiceVersion(httpMessage.getValueByName("SERVICE-VER"));
    service->setIp(ip);
    service->setPort(port);
    service->setRetry(0);
    service->setNextNotifyTime(now + timeout);

    if (this->addService(service))
        m_handler->onServiceFound(service);

    // 打印列表
#ifdef SDP_DISCOVERY_DEBUG
    std::cout << "[ " << now << " ] " <<"online: " << service->deviceName() << " nextnotifytime: " << service->nextNotifyTime() << std::endl;

    ServiceInfoList::iterator iter = m_services.begin();
    for (; iter != m_services.end(); ++iter)
        std::cout << (*iter)->deviceName() << " < ";
    std::cout << std::endl;
#endif
}

void SdpDiscoveryWorker::handleServiceLost(const std::string &ip, uint16_t port, HttpMessage &httpMessage)
{
	if (m_isStartDiscover == false)
		return;
	
	if (httpMessage.getValueByName("SERVICE-TYPE") != m_serviceType)
        return;

    boost::mutex::scoped_lock lock(m_servicesMutex);
    SdpServiceInfoPointer p = this->getServiceByIp(m_services, ip);
    if (p.use_count() == 0)
        return;

    m_handler->onServiceLost(p);
}

/*
* 添加服务
* 如果
* 添加成功，返回true
* 添加失败，返回false
*/
bool SdpDiscoveryWorker::addService(SdpServiceInfoPointer newService)
{
    boost::mutex::scoped_lock lock(m_servicesMutex);
    SdpServiceInfoPointer p = this->getServiceById(m_services, newService->deviceId());

    // 将其放入列表最后
    m_services.push_back(newService);

    // 列表中找不到此服务，认为是一个新服务
    if (p.use_count() == 0)
        return true;

    // 如果是旧服务
    if (p->deviceName() != newService->deviceName() 
        || p->ip() != newService->ip() 
        || p->servicePort() != newService->servicePort())
    {
        // 服务信息已经更新
        return true;
    }

    // 服务信息未更新
    return false;
}

SdpServiceInfoPointer SdpDiscoveryWorker::getServiceById(ServiceInfoList &list, const std::string &deviceId)
{
    SdpServiceInfoPointer pService;

    ServiceInfoList::iterator p = list.begin();
    for (; p != list.end(); ++p)
    {
        if ((*p)->deviceId() == deviceId)
        {
            pService = *p;
            list.erase(p);
            break;
        }
    }

    return pService;
}

SdpServiceInfoPointer SdpDiscoveryWorker::pickServiceById(ServiceInfoList &list, const std::string &deviceId)
{
    SdpServiceInfoPointer pService;

    ServiceInfoList::iterator p = list.begin();
    for (; p != list.end(); ++p)
    {
        if ((*p)->deviceId() == deviceId)
        {
            pService = *p;
            break;
        }
    }

    return pService;
}


SdpServiceInfoPointer SdpDiscoveryWorker::getServiceByIp(ServiceInfoList &list, const std::string &ip)
{
    SdpServiceInfoPointer pService;

    ServiceInfoList::iterator p = list.begin();
    for (; p != list.end(); ++p)
    {
        if ((*p)->ip() == ip)
        {
            pService = *p;
            list.erase(p);
            break;
        }
    }

    return pService;
}

SdpServiceInfoPointer SdpDiscoveryWorker::pickServiceByIp(ServiceInfoList &list, const std::string &ip)
{
    SdpServiceInfoPointer pService;

    ServiceInfoList::iterator p = list.begin();
    for (; p != list.end(); ++p)
    {
        if ((*p)->ip() == ip)
        {
            pService = *p;
            break;
        }
    }

    return pService;
}
