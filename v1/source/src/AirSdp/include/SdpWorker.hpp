/*
* SdpWorker.hpp
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

#ifndef __SDP_WORKER_H__
#define __SDP_WORKER_H__

#include "TheBoost.hpp"

#include "SdpMasterHandler.hpp"
#include "SdpPublishWorker.hpp"
#include "SdpDiscoveryWorker.hpp"

namespace airjoy
{

    class SdpWorker : public SdpMasterHandler
    {
    public:
        typedef boost::shared_ptr<SdpPublishWorker>     SdpPublishWorkerPointer;
        typedef boost::shared_ptr<SdpDiscoveryWorker>   SdpDiscoveryWorkerPointer;
    
    private:
        SdpPublishWorkerPointer     m_publishWorker;
        SdpDiscoveryWorkerPointer   m_discoveryWorker;

    public:
        SdpWorker()
        {
        }

        virtual ~SdpWorker()
        {
        }

        void setPublishWorker(SdpPublishWorkerPointer w)
        {
            m_publishWorker = w;
        }

        void setDiscoveryWorker(SdpDiscoveryWorkerPointer w)
        {
            m_discoveryWorker = w;
        }

        // SdpMasterhandler
        virtual uint64_t getPublishTimeout(SdpMaster *master)
        {
            return m_publishWorker->getPublishTimeout(master);
        }

        virtual void onPublishTimeout(SdpMaster *master)
        {
            m_publishWorker->onTimeout(master);
        }

        virtual uint64_t getDiscoverTimeout(SdpMaster *master)
        {
            return m_discoveryWorker->getDiscoverTimeout(master);
        }

        virtual void onDiscoverTimeout(SdpMaster *master)
        {
            m_discoveryWorker->onDiscoverTimeout(master);
        }
        
        virtual uint64_t getServiceTimeout(SdpMaster *master)
        {
            return m_discoveryWorker->getServiceTimeout(master);
        }
        
        virtual void onServiceTimeout(SdpMaster *master)
        {
            m_discoveryWorker->onServiceTimeout(master);
        }

        virtual void onReceived(SdpMaster *master, const std::string &ip, uint16_t port, const std::string &buf)
        {
            m_discoveryWorker->onReceived(master, ip, port, buf);
        }
    };

}

#endif // __SDP_WORKER_H__
