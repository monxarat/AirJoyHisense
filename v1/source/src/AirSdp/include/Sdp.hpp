/*
* Sdp.hpp
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

#ifndef __SDP_H__
#define __SDP_H__

#include "TheBoost.hpp"
#include "SdpDiscoveryWorker.hpp"
#include "SdpMaster.hpp"
#include "SdpWorker.hpp"
#include "HttpMessage.hpp"
#include "SdpServiceInfo.hpp"

namespace airjoy
{

    class Sdp
    {
    public:
        typedef boost::shared_ptr<SdpMaster>            SdpMasterPointer;
        typedef boost::shared_ptr<SdpWorker>            SdpWorkerPointer;
        typedef boost::shared_ptr<SdpPublishWorker>     SdpPublishWorkerPointer;
        typedef boost::shared_ptr<SdpDiscoveryWorker>   SdpDiscoveryWorkerPointer;
        typedef SdpDiscoveryWorker::SdpDiscoveryHandlerPointer  SdpDiscoveryHandlerPointer;

    private:
        SdpMasterPointer            m_master;
        SdpWorkerPointer            m_worker;
        SdpPublishWorkerPointer     m_publishWorker;
        SdpDiscoveryWorkerPointer   m_discoveryWorker;

        SdpServiceInfoPointer       m_publishedService;
        bool                        m_hasPublishedService;

    public:
        Sdp(boost::asio::io_service &ios, int port)
        {
            m_master = boost::make_shared<SdpMaster>(boost::ref(ios), port);

            m_worker = boost::make_shared<SdpWorker>();
            m_publishWorker = boost::make_shared<SdpPublishWorker>();
            m_discoveryWorker = boost::make_shared<SdpDiscoveryWorker>();

            m_worker->setPublishWorker(m_publishWorker);
            m_worker->setDiscoveryWorker(m_discoveryWorker);

            m_master->setHandler(m_worker);
            m_hasPublishedService = false;
        }

        virtual ~Sdp()
        {
        }

        void setServiceTimeout(int second)
        {
            m_discoveryWorker->setServiceOnlineTimeout(second);
        }

        bool start()
        {
            return m_master->start();
        }

        bool stop()
        {
            if (m_hasPublishedService)
                unPublishService(m_publishedService);

            return m_master->stop();
        }

        bool discoveryService(const std::string & type,
                              SdpDiscoveryHandlerPointer handler)
        {
            return m_discoveryWorker->startDiscover(type, handler);
        }

        bool stopServiceDiscovery(void)
        {
            return m_discoveryWorker->stopDiscover();
        }

        bool publishService(SdpServiceInfoPointer service)
        {
            m_hasPublishedService = true;
            m_publishedService = service;

            m_discoveryWorker->addPublishedService(service);

            m_publishWorker->publishService(service);

            // 发送服务在线宣告
            std::string timeout;
            std::string servicePort;
            std::string deviceType;

            try 
            {
                timeout = boost::lexical_cast<std::string>(service->notifyTimeout());
                servicePort = boost::lexical_cast<std::string>(service->servicePort());
                deviceType = boost::lexical_cast<std::string>(service->deviceType());
            }
            catch (boost::bad_lexical_cast&)
            {
                std::cout << "Sdp::publishService error!" << std::endl;

                return false;
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
            httpRequest.addHeaders("SERVICE-TYPE", service->serviceType());
            httpRequest.addHeaders("SERVICE-PORT", servicePort);
            httpRequest.addHeaders("SERVICE-VER", service->serviceVersion());
            httpRequest.addHeaders("DEVICE-ID", service->deviceId());
            httpRequest.addHeaders("DEVICE-TYPE", deviceType);
            httpRequest.addHeaders("DEVICE-NAME", service->deviceName());

            m_master->sendToAll(httpRequest.toString());

            return true;
        }

        void unPublishService(SdpServiceInfoPointer service)
        {
            m_hasPublishedService = false;

            m_discoveryWorker->removePublishedService(service);

            m_publishWorker->unPublishService(service);

            // 发送服务离线宣告
            HttpMessage httpRequest;
            httpRequest.setHttpType(HttpRequest);
            httpRequest.setHttpVersionMajor(1);
            httpRequest.setHttpVersionMinor(1);
            httpRequest.setHttpMethod("NOTIFY");
            httpRequest.setUri("*");

            httpRequest.addHeaders("HOST", "239.255.255.250:1900");
            httpRequest.addHeaders("MAN", "airjoy:sdp/1.0");
            httpRequest.addHeaders("NTS", "sdp:byebye");
            httpRequest.addHeaders("SERVICE-TYPE", service->serviceType());

            m_master->sendToAll(httpRequest.toString());
        }

#if 0
        SdpServiceInfoPointer getServiceInfo(const std::string &deviceId)
        {
            return m_discoveryWorker->getServiceInfo(deviceId);
        }
#endif

        SdpServiceInfoPointer getSelfServiceInfo(void)
        {
            return m_publishedService;
        }
    };


}

#endif // __SDP_H__
