/*
* Nsd.hpp
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

#ifndef __NSD_H__
#define __NSD_H__

#include "TheBoost.hpp"

#include <string>
#include <list>
#include <dns_sd.h>

namespace airjoy
{

    class NsdServiceInfo;
    class NsdDiscoveryHandler;
    class NsdPublishHandler;

    class Nsd
    {
    public:
        typedef boost::shared_ptr<NsdDiscoveryHandler>  NsdDiscoveryHandlerPointer;
        typedef boost::shared_ptr<NsdPublishHandler>    NsdPublishHandlerPointer;
        typedef boost::shared_ptr<NsdServiceInfo>       NsdServiceInfoPointer;
        typedef boost::shared_ptr<boost::thread>        ThreadPointer;

        typedef boost::mutex                        MutexType;
        typedef MutexType::scoped_lock              LockType;

    public:
        DNSServiceRef   m_shareRef;
        DNSServiceRef   m_browseRef;
        DNSServiceRef   m_resolveRef;
        DNSServiceRef   m_resolvAddrRef;
        DNSServiceRef   m_registerRef;

    private:
        std::string                     m_discoveryServiceType;

        NsdDiscoveryHandlerPointer      m_discoveryHandler;
        NsdPublishHandlerPointer        m_publishHandler;
        NsdServiceInfoPointer           m_publishService;
        
        typedef std::list<NsdServiceInfoPointer>   ServiceList;
        ServiceList                    m_services;
        MutexType                      m_servicesMutex;

        ThreadPointer             m_pThread;
        bool                      m_threadShouldExit;

    public:
        Nsd();
        ~Nsd();

        bool discoveryService(const std::string & serviceType, 
                              const std::string & domainName,
                              NsdDiscoveryHandlerPointer handler);

        bool stopServiceDiscovery(void);
         
        // must called after discoveryService()
        bool publishService(NsdServiceInfoPointer serviceInfo, NsdPublishHandlerPointer handler);
        bool unpublishService();
        
        bool doForceReAnnounceService(NsdServiceInfoPointer serviceInfo);

        NsdDiscoveryHandlerPointer discoveryHandler(void) { return m_discoveryHandler; }
        NsdPublishHandlerPointer publishHandler(void) { return m_publishHandler; }

    public:
        void addService(const char *name, const char *type, const char *domain);
        void removeService(const char *name, const char *type, const char *domain);
        void removeAllService(void);

        bool updateServiceInfo(const char *fullResolveName,
                               const char *hostName,
                               uint16_t    port,
                               uint16_t    txtLen,
                               const unsigned char *txtRecord);

        void updateIpAddressWithHostName(const char *hostName, 
                                         const struct sockaddr *address, 
                                         uint32_t ttl);

        NsdServiceInfoPointer getPublishService(void) { return m_publishService; }
        
        const std::string & discoveryServiceType(void) { return m_discoveryServiceType; }

    private:
        DNSServiceErrorType startServiceBrowse(const char *serviceType,
                                               const char *domain,
                                               void *context);

        void HandleEvents (DNSServiceRef *ref);

        bool poll(DNSServiceRef *dnsServiceRef, double timeOutInSeconds, DNSServiceErrorType &err);
        void thread(DNSServiceRef *dnsServiceRef, double timeOutInSeconds);
    };

}


#endif // __NSD_H__
