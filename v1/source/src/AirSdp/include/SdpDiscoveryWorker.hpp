/*
* SdpDiscoveryWorker.hpp
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

#ifndef __SDP_DISCOVERY_WORKER_H__
#define __SDP_DISCOVERY_WORKER_H__

#include <string>
#include <map>
#include <list>
#include "TheBoost.hpp"
#include "SdpDiscoveryHandler.hpp"
#include "HttpMessage.hpp"

#ifdef _WIN32
#else
#include "AirTimer.hpp"
#endif // _WIN32

namespace airjoy
{

    class SdpMaster;

    class SdpDiscoveryWorker
    {
    public:
        typedef boost::shared_ptr<SdpDiscoveryHandler>          SdpDiscoveryHandlerPointer;
        typedef std::list<SdpServiceInfoPointer>                ServiceInfoList;

    private:
		SdpDiscoveryHandlerPointer      m_handler;

        // services
        ServiceInfoList                 m_services;
        boost::mutex                    m_servicesMutex;

        // published service
        SdpServiceInfoPointer           m_publishedService;
        bool                            m_hasPublishedService;

        std::string                     m_serviceType;
        bool                            m_isStartDiscover;

        // o'clock
    #ifdef _WIN32
        boost::timer                    m_clock;
    #else
        AirTimer                        m_clock;
    #endif // _WIN32

        // service online timeout
        uint64_t                        m_serviceTimeout;
        uint64_t                        m_discoverTimeout;

    public:
        SdpDiscoveryWorker();
        virtual ~SdpDiscoveryWorker();
        
        // 在线有效时间
        void setServiceOnlineTimeout(int second);

        bool startDiscover(const std::string & serviceType, SdpDiscoveryHandlerPointer handler);        
        bool stopDiscover(void);

        uint64_t getDiscoverTimeout(SdpMaster *master);
        void onDiscoverTimeout(SdpMaster *master);

        uint64_t getServiceTimeout(SdpMaster *master);
        void onServiceTimeout(SdpMaster *master);

        void onReceived(SdpMaster *master, const std::string &ip, uint16_t port, const std::string &buf);

        void addPublishedService(SdpServiceInfoPointer service);
        void removePublishedService(SdpServiceInfoPointer service);

    private:
        uint64_t nowClock(void);
        void searchService(SdpMaster *master);
        void searchService(SdpMaster *master, SdpServiceInfoPointer service);

        void handleMsearch(SdpMaster *master, const std::string &ip, uint16_t port, HttpMessage &httpMessage);
        void handleServiceFound(const std::string &ip, uint16_t port, HttpMessage &httpMessage);
        void handleServiceLost(const std::string &ip, uint16_t port, HttpMessage &httpMessage);

        bool addService(SdpServiceInfoPointer newService);

        SdpServiceInfoPointer getServiceById(ServiceInfoList &list, const std::string &deviceId);
        SdpServiceInfoPointer pickServiceById(ServiceInfoList &list, const std::string &deviceId);

        SdpServiceInfoPointer getServiceByIp(ServiceInfoList &list, const std::string &ip);
        SdpServiceInfoPointer pickServiceByIp(ServiceInfoList &list, const std::string &ip);

    };

}

#endif // __SDP_DISCOVERY_WORKER_H_
