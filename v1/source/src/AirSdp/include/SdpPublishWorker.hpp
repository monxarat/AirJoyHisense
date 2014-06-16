/*
* SdpPulibshWorker.hpp
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

#ifndef __SDP_PUBLISH_WORKER_H__
#define __SDP_PUBLISH_WORKER_H__


#include "TheBoost.hpp"
#include "SdpServiceInfo.hpp"

namespace airjoy
{

    class SdpMaster;

    class SdpPublishWorker
    {
    private:
        SdpServiceInfoPointer       m_serviceInfo;
        bool                        m_needPublish;

    public:
        SdpPublishWorker();
        virtual ~SdpPublishWorker();

        bool publishService(SdpServiceInfoPointer service);
        bool unPublishService(SdpServiceInfoPointer service);

        uint64_t getPublishTimeout(SdpMaster *master);
        void onTimeout(SdpMaster *master);
    };

}

#endif // __SDP_PUBLISH_WORKER_H__
