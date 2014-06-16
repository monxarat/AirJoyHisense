/*
* Nsd.cpp
*
* Net Service Discovery
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

#include "Nsd.hpp"
#include "NsdServiceInfo.hpp"
#include "NsdDiscoveryHandler.hpp"
#include "NsdPublishHandler.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#else
#include <arpa/inet.h>
#endif

#include <iostream> // for debug

using namespace airjoy;

//-----------------------------------------------------------------------------
// Static API
//-----------------------------------------------------------------------------

static 
void DNSSD_API address_reply (DNSServiceRef sdRef,
                              DNSServiceFlags flags, 
                              uint32_t interfaceIndex, 
                              DNSServiceErrorType errorCode, 
                              const char *hostname, 
                              const struct sockaddr *address, 
                              uint32_t ttl, 
                              void *context)
{
    //std::cout << " ---> address_reply" << std::endl;

    Nsd *self = (Nsd *)context;
    self->updateIpAddressWithHostName(hostname, address, ttl);
}

static
void DNSSD_API resolve_reply (DNSServiceRef sdRef,
                              DNSServiceFlags flags, 
                              uint32_t interfaceIndex, 
                              DNSServiceErrorType errorCode,
                              const char *fullresolvename, 
                              const char *hosttarget, 
                              uint16_t opaqueport,            ///< port 
                              uint16_t txtLen,
                              const unsigned char *txtRecord, 
                              void *context)
{
    //std::cout << " --> resolve_reply" << std::endl;

    Nsd *self = (Nsd *)context;
    self->updateServiceInfo(fullresolvename, hosttarget, opaqueport, txtLen, txtRecord);

    self->m_resolvAddrRef = self->m_shareRef;

    DNSServiceGetAddrInfo (&self->m_resolvAddrRef,
                            kDNSServiceFlagsShareConnection,
                            interfaceIndex,
                            kDNSServiceProtocol_IPv4,
                            hosttarget,
                            address_reply,
                            context);
}

static 
void DNSSD_API browse_reply (DNSServiceRef sdref, 
                             const DNSServiceFlags flags, 
                             uint32_t ifIndex, 
                             DNSServiceErrorType errorCode,
                             const char *replyName, 
                             const char *replyType, 
                             const char *replyDomain, 
                             void *context)
{
    //std::cout << " -> browse_reply" << std::endl;

    Nsd *self = (Nsd *)context;

    if (flags & kDNSServiceFlagsAdd) 
    {
        self->addService(replyName, replyType, replyDomain);

        self->m_resolveRef = self->m_shareRef;

        DNSServiceResolve (&(self->m_resolveRef),
            kDNSServiceFlagsShareConnection,
            ifIndex,
            replyName,
            replyType,
            replyDomain,
            resolve_reply,
            context);
    }
    else 
    {
        self->removeService(replyName, replyType, replyDomain);
    }
}

static
void DNSSD_API register_reply(DNSServiceRef       sdRef,
                              DNSServiceFlags     flags, 
                              DNSServiceErrorType errorCode, 
                              const char          *name, 
                              const char          *regtype, 
                              const char          *domain, 
                              void                *context)
{
    Nsd *self = (Nsd *)context;

    switch (errorCode)
    {
    case kDNSServiceErr_NoError:
        self->publishHandler()->didPublish(self->getPublishService());
        break;

    case kDNSServiceErr_NameConflict: 
        self->publishHandler()->didNotPublish(self->getPublishService());
        break;

    default:                          
        self->publishHandler()->didNotPublish(self->getPublishService());
        break;
    }
}  

//-----------------------------------------------------------------------------
// Public API
//-----------------------------------------------------------------------------

Nsd::Nsd() 
    : m_shareRef(NULL)
    , m_browseRef(NULL)
    , m_resolveRef(NULL)
    , m_resolvAddrRef(NULL)
    , m_registerRef(NULL)
    , m_threadShouldExit(false)
{
}

Nsd::~Nsd() 
{
    stopServiceDiscovery();
}

bool Nsd::discoveryService(const std::string &serviceType, 
                           const std::string &domainName,
                           NsdDiscoveryHandlerPointer handler)
{
    m_discoveryServiceType = serviceType;
    m_discoveryHandler = handler;

    stopServiceDiscovery();

    DNSServiceErrorType err = this->startServiceBrowse(serviceType.c_str(), domainName.c_str(), this);
    if (err != kDNSServiceErr_NoError)
    {
        //m_discoveryHandler->didNotStart(this);

        if (m_shareRef)
        {
            DNSServiceRefDeallocate(m_shareRef);
            m_shareRef = NULL;
        }

        return false;
    }

    //m_discoveryHandler->willStart(this);

    try
    {
        m_pThread = boost::make_shared<boost::thread>(boost::bind(&Nsd::thread, this, &m_shareRef, 1.0));
    }
    catch (std::exception &e)
    {
        std::cout << "Nsd::startMonitor: " << e.what() << std::endl;
        return false;
    }

    //m_discoveryHandler->didStart(this);

    return true;
}

bool Nsd::publishService(NsdServiceInfoPointer serviceInfo, NsdPublishHandlerPointer handler)
{
    if (m_pThread.use_count() == 0)
    {
        //handler->didNotPublish(serviceInfo);
        return false;
    }

    if (serviceInfo->getPort() == 0)
    {
        //handler->didNotPublish(serviceInfo);
        return false;
    }

    m_publishHandler = handler;
    m_publishService = serviceInfo;

    DNSServiceFlags flags	= kDNSServiceFlagsShareConnection;
    uint32_t interfaceIndex = kDNSServiceInterfaceIndexAny;		// all interfaces 

    const char *name = serviceInfo->getName().c_str();
    const char *type = serviceInfo->getType().c_str();
    const char *domain = serviceInfo->getDomain().c_str();
    const char *hostName = serviceInfo->getHostName().c_str();
    //uint16_t registerPort = htons(serviceInfo->getPort());

    //std::cout << "getPort: " << serviceInfo->getPort() << std::endl;
    //std::cout << "registerPort: " << registerPort << std::endl;

    // TXTRecord
    TXTRecordRef txtRecord;
    TXTRecordCreate(&txtRecord, 0, NULL);

#if 0
    std::string features    = serviceInfo->getTXTRecordValueWithKey("features");
    std::string model       = serviceInfo->getTXTRecordValueWithKey("model");
    std::string deviceid    = serviceInfo->getTXTRecordValueWithKey("deviceid");
    std::string srcvers     = serviceInfo->getTXTRecordValueWithKey("srcvers");

    DNSServiceErrorType err = kDNSServiceErr_NoError;
    err = TXTRecordSetValue(&txtRecord, "features", features.size(), features.c_str());
    err = TXTRecordSetValue(&txtRecord, "model", model.size(), model.c_str());
    err = TXTRecordSetValue(&txtRecord, "deviceid", deviceid.size(), deviceid.c_str());
    err = TXTRecordSetValue(&txtRecord, "srcvers", srcvers.size(), srcvers.c_str());
#else
    NsdServiceInfo::TXTRecordMap &txt = serviceInfo->txtRecord();
    NsdServiceInfo::TXTRecordMap::iterator iter = txt.begin();
    for (; iter != txt.end(); ++iter)
    {
        const char * key = (*iter)->getKey();
        const char * value = (*iter)->getValue();

        DNSServiceErrorType err = kDNSServiceErr_NoError;
        err = TXTRecordSetValue(&txtRecord, key, strlen(value), value);

        // std::cout << key << "=" << value << std::endl;
    }

#if 0
    if (serviceInfo->getType() == std::string("_airplay._tcp"))
    {
        if (serviceInfo->getUpdateNumber() % 2 == 0)
        {
            TXTRecordSetValue(&txtRecord, "ouyang", strlen("178915185"), "178915185");
            //TXTRecordSetValue(&txtRecord, "xbmcdummy", strlen("evendummy"), "evendummy");
            //std::cout << "xbmcdummy" << "=" << "evendummy" << std::endl;
        }
        else
        {
            TXTRecordSetValue(&txtRecord, "ouyang", strlen("jxfengzi"), "jxfengzi");
            //TXTRecordSetValue(&txtRecord, "xbmcdummy", strlen("odddummy"), "odddummy");
            //std::cout << "xbmcdummy" << "=" << "odddummy" << std::endl;
        }

        serviceInfo->increaseUpdateNumber();
    }
#endif
#endif

    m_registerRef = m_shareRef;

    DNSServiceErrorType result = DNSServiceRegister(&m_registerRef,
                                                     flags, 
                                                     interfaceIndex, 
                                                     name,
                                                     type,
                                                     domain,
                                                     hostName, 
                                                     htons(serviceInfo->getPort()),
                                                     TXTRecordGetLength (&txtRecord),
                                                     TXTRecordGetBytesPtr (&txtRecord),
                                                     (DNSServiceRegisterReply)&register_reply, 
                                                     this);
  
    TXTRecordDeallocate(&txtRecord);

    if (result != kDNSServiceErr_NoError)
    {
        //m_publishHandler->didNotPublish(serviceInfo);

        if (m_registerRef)
        {
            DNSServiceRefDeallocate(m_registerRef);
            m_registerRef = NULL;			
        }

        return false;
    }

    //m_publishHandler->willPublish(serviceInfo);

    return true;
}

bool Nsd::unpublishService()
{
    if (m_registerRef == NULL) 
        return false;

    return true;
}

bool Nsd::doForceReAnnounceService(NsdServiceInfoPointer serviceInfo)
{
    bool ret = false;
    
    if (serviceInfo->getType() != std::string("_airplay._tcp"))
    {
        //std::cout << "NOT doForceReAnnounceService: " << serviceInfo->getName() << std::endl;
        return ret;
    }

    //std::cout << "doForceReAnnounceService: " << serviceInfo->getName() << std::endl;

    // TXTRecord
    TXTRecordRef txtRecord;
    TXTRecordCreate(&txtRecord, 0, NULL);

    // for force announcing a service with mdns we need
    // to change a txt record - so we diddle between
    // even and odd dummy records here

    NsdServiceInfo::TXTRecordMap &txt = serviceInfo->txtRecord();
    NsdServiceInfo::TXTRecordMap::iterator iter = txt.begin();
    for (; iter != txt.end(); ++iter)
    {
        const char * key = (*iter)->getKey();
        const char * value = (*iter)->getValue();

        DNSServiceErrorType err = kDNSServiceErr_NoError;
        err = TXTRecordSetValue(&txtRecord, key, strlen(value), value);

        // std::cout << key << "=" << value << std::endl;
   }

#if 0
    if (serviceInfo->getType() == std::string("_airplay._tcp"))
    {
        if (serviceInfo->getUpdateNumber() % 2 == 0)
        {
            TXTRecordSetValue(&txtRecord, "ouyang", strlen("178915185"), "178915185");
            //TXTRecordSetValue(&txtRecord, "xbmcdummy", strlen("evendummy"), "evendummy");
            //std::cout << "xbmcdummy" << "=" << "evendummy" << std::endl;
        }
        else
        {
            TXTRecordSetValue(&txtRecord, "ouyang", strlen("jxfengzi"), "jxfengzi");
            //TXTRecordSetValue(&txtRecord, "xbmcdummy", strlen("odddummy"), "odddummy");
            //std::cout << "xbmcdummy" << "=" << "odddummy" << std::endl;
        }

        serviceInfo->increaseUpdateNumber();
    }
#endif

    DNSServiceErrorType result = DNSServiceUpdateRecord(m_registerRef, 
        NULL, 
        0, 
        TXTRecordGetLength(&txtRecord), 
        TXTRecordGetBytesPtr(&txtRecord), 
        0);

    if (result ==  kDNSServiceErr_NoError)
          ret = true;

    return ret;
}

bool Nsd::stopServiceDiscovery()
{
    if (m_pThread.use_count() > 0)
    {
        m_threadShouldExit = true;

        m_pThread->join();
        m_pThread.reset();
    }

    if (m_shareRef)
    {
        DNSServiceRefDeallocate(m_shareRef);
        m_shareRef = NULL;
    }

    if (m_browseRef)
    {
        DNSServiceRefDeallocate(m_browseRef);
        m_browseRef = NULL;
    }

    if (m_resolveRef)
    {
        DNSServiceRefDeallocate(m_resolveRef);
        m_resolveRef = NULL;
    }

    if (m_resolvAddrRef)
    {
        DNSServiceRefDeallocate(m_resolvAddrRef);
        m_resolvAddrRef = NULL;
    }

    if (m_registerRef)
    {
        DNSServiceRefDeallocate(m_registerRef);
        m_registerRef = NULL;
    }

    return true;
}

//-----------------------------------------------------------------------------
// Private API
//-----------------------------------------------------------------------------

DNSServiceErrorType Nsd::startServiceBrowse(const char *serviceType,
                                                          const char *domain,
                                                          void *context)
{
    uint32_t interfaceIndex = kDNSServiceInterfaceIndexAny;
    DNSServiceErrorType err;

    DNSServiceFlags flag = kDNSServiceFlagsShareConnection;

    err = DNSServiceCreateConnection (&m_shareRef);
    if (err != kDNSServiceErr_NoError)
        return err;

    m_browseRef = m_shareRef;

    return DNSServiceBrowse (&m_browseRef,
                              flag,
                              interfaceIndex,
                              serviceType,
                              domain,
                              browse_reply,
                              context);
}

void Nsd::HandleEvents (DNSServiceRef *serviceRef)
{
    int dns_sd_fd  = *serviceRef ? DNSServiceRefSockFD(*serviceRef   ) : -1;
    int nfds = dns_sd_fd + 1;
    fd_set readfds;
    struct timeval tv;
    int result;

    // 1. Set up the fd_set as usual here.
    // This example thiz->dnsSdRef has no file descriptors of its own,
    // but a real application would call FD_SET to add them to the set here
    FD_ZERO(&readfds);

    // 2. Add the fd for our thiz->dnsSdRef(s) to the fd_set
    FD_SET(dns_sd_fd , &readfds);

    // 3. Set up the timeout.
    //tv.tv_sec  = LONG_TIME;
    tv.tv_sec  = 3;
    tv.tv_usec = 0;

    result = select(nfds, &readfds, (fd_set*)NULL, (fd_set*)NULL, &tv);
    if (result > 0) 
    {
        DNSServiceErrorType err = kDNSServiceErr_NoError;
        if ((*serviceRef) && FD_ISSET(dns_sd_fd , &readfds)) 
            err = DNSServiceProcessResult (*serviceRef);
    }
}

void Nsd::addService(const char *name, const char *type, const char *domain)
{
    ServiceList::iterator iter;
    for (iter = m_services.begin(); iter != m_services.end(); ++iter)
    {
        NsdServiceInfoPointer p = *iter;
        if (p->getName() == name && p->getType() == type && p->getDomain() == domain)
            return;
    }

    // add new service
    NsdServiceInfoPointer newService = boost::make_shared<NsdServiceInfo>(name, type, domain);

    m_discoveryHandler->didAddService(newService);

    LockType lock(m_servicesMutex);
    m_services.push_back(newService);
}

void Nsd::removeService(const char *name, const char *type, const char *domain)
{
    LockType lock(m_servicesMutex);

    ServiceList::iterator iter;
    for (iter = m_services.begin(); iter != m_services.end(); ++iter)
    {
        NsdServiceInfoPointer p = *iter;
        if (p->getName() == name && p->getType() == type && p->getDomain() == domain)
        {
            m_discoveryHandler->didRemoveService(p);

            m_services.erase(iter);
            break;
        }
    }
}

void Nsd::removeAllService(void)
{
    LockType lock(m_servicesMutex);
    m_services.clear();
}

bool Nsd::updateServiceInfo(const char *fullResolveName, 
                            const char *hostName,
                            uint16_t    port,
                            uint16_t    txtLen,
                            const unsigned char *txtRecord)
{
    LockType lock(m_servicesMutex);

    ServiceList::iterator iter;
    for (iter = m_services.begin(); iter != m_services.end(); ++iter)
    {
        NsdServiceInfoPointer p = *iter;
        if (p->getFullResolveName() == fullResolveName)
        {
            p->setHostName(hostName);
            p->setPort(ntohs(port));
            p->setTXTRecordData(txtLen, txtRecord);
            m_discoveryHandler->didResolvService(p);

            return true;
        }
    }

    return false;
}

void Nsd::updateIpAddressWithHostName(const char *hostName,
                                      const struct sockaddr *address,
                                      uint32_t ttl)
{
    LockType lock(m_servicesMutex);

    ServiceList::iterator iter;
    for (iter = m_services.begin(); iter != m_services.end(); ++iter)
    {
        NsdServiceInfoPointer p = *iter;
        if (p->getHostName() == hostName)
        {
            p->setIPAddress(address, ttl);

            m_discoveryHandler->didGetIPAddressForService(p);

            break;
        }
    }
}

bool Nsd::poll(DNSServiceRef *dnsServiceRef, double timeOutInSeconds, DNSServiceErrorType &err)
{
    assert(dnsServiceRef);

    err = kDNSServiceErr_NoError;

    fd_set readfds;
    FD_ZERO(&readfds);

    int dns_sd_fd = DNSServiceRefSockFD(*dnsServiceRef);
    int nfds = dns_sd_fd + 1;
    FD_SET(dns_sd_fd, &readfds);

    struct timeval tv;
    tv.tv_sec = long(floor(timeOutInSeconds));
    tv.tv_usec = long(1000000*(timeOutInSeconds - tv.tv_sec));

    int result = select(nfds, &readfds, NULL, NULL, &tv);
    if (result>0 && FD_ISSET(dns_sd_fd, &readfds))
    {
        err = DNSServiceProcessResult(*dnsServiceRef);
        return true;
    }

    return false;
}

void Nsd::thread(DNSServiceRef *dnsServiceRef, double timeOutInSeconds)
{
    while (! m_threadShouldExit)
    {
        DNSServiceErrorType err = kDNSServiceErr_NoError;
        if (poll(dnsServiceRef, timeOutInSeconds, err))
        {
            if(err>0)
                m_threadShouldExit = true;
        }
    }
}
