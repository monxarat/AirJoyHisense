/*
 * AirPlayNsd.cpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-15
 *
 *
 */

#include "AirPlayNsd.hpp"
#include "AirPlayNsdCore.hpp"
#include "AirPlayNsdPublishHandler.hpp"
#include "AirPlayNsdDiscoveryHandler.hpp"
#include "NsdServiceInfo.hpp"

using namespace airjoy;

AirPlayNsd::AirPlayNsd()
    :m_isStart(false)
{
     m_core = new AirPlayNsdCore;
}

AirPlayNsd::~AirPlayNsd()
{
    delete m_core;
}

bool AirPlayNsd::discoveryService(const std::string &serviceType, AirPlayNsdDiscoveryListener *listener)
{
    if (m_isStart)
        return true;
    
    m_isStart = true;

    std::string domainName("local.");

    Nsd::NsdDiscoveryHandlerPointer discoveryHandler = boost::make_shared<AirPlayNsdDiscoveryHandler>(listener);
    bool ret = m_core->nsd()->discoveryService(serviceType, domainName, discoveryHandler);

    m_core->start();

    return ret;
}

bool AirPlayNsd::stopServiceDiscovery(void)
{
    if (! m_isStart)
        return false;
    
    m_isStart = false;

    m_core->nsd()->stopServiceDiscovery();

    return true;
}

bool AirPlayNsd::publishService(AirPlayServiceInfo &serviceInfo, AirPlayNsdPublishListener *listener)
{
    if (! m_isStart)
        return true;

    std::string domainName("local.");
    Nsd::NsdServiceInfoPointer s = boost::make_shared<NsdServiceInfo>(serviceInfo.deviceName(), serviceInfo.type(), domainName);

    // 设置端口
    s->setPort(serviceInfo.port());

    std::stringstream ssFeatures;
    ssFeatures << "0x" << std::hex << serviceInfo.features();

    s->setTXTRecordValueWithKey("deviceid", serviceInfo.deviceId().c_str());
    s->setTXTRecordValueWithKey("features", ssFeatures.str().c_str());
    s->setTXTRecordValueWithKey("model", serviceInfo.deviceModel().c_str());
    s->setTXTRecordValueWithKey("rhd", "1.9.8");
    s->setTXTRecordValueWithKey("srcvers", serviceInfo.version().c_str());
    s->setTXTRecordValueWithKey("vv", "1");

    m_core->addService(s);

    Nsd::NsdPublishHandlerPointer publishHandler = boost::make_shared<AirPlayNsdPublishHandler>(listener);
    return m_core->nsd()->publishService(s, publishHandler);
}

bool AirPlayNsd::publishService(AirTunesServiceInfo &serviceInfo, AirPlayNsdPublishListener *listener)
{
    if (! m_isStart)
        return true;

    std::string domainName("local.");
    Nsd::NsdServiceInfoPointer s = boost::make_shared<NsdServiceInfo>(serviceInfo.publishedName(), serviceInfo.type(), domainName);

    // 设置端口
    s->setPort(serviceInfo.port());

    AirTunesServiceInfo::TxtRecordType &txt = serviceInfo.txt();
    AirTunesServiceInfo::TxtRecordType::iterator iter = txt.begin();

    for (; iter != txt.end(); ++iter)
	{
        //s->setTXTRecordValueWithKey(iter->first.c_str(), iter->second.c_str());
		s->setTXTRecordValueWithKey((*iter)->getKey(), (*iter)->getValue());
	}

    m_core->addService(s);

    Nsd::NsdPublishHandlerPointer publishHandler = boost::make_shared<AirPlayNsdPublishHandler>(listener);
    return m_core->nsd()->publishService(s, publishHandler);
}


bool AirPlayNsd::unpublishService()
{
    m_core->removeAllService();

    return m_core->nsd()->unpublishService();
}
