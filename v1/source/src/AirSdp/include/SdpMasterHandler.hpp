/*
* SdpMasterHandler.hpp
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

#ifndef __SDP_MASTER_HANDLER_H__
#define __SDP_MASTER_HANDLER_H__

#include <string>

namespace airjoy
{

    class SdpMaster;

    class SdpMasterHandler
    {
    public:
        SdpMasterHandler() {}
        virtual ~SdpMasterHandler() {}

        virtual uint64_t getPublishTimeout(SdpMaster *master) = 0;
        virtual void onPublishTimeout(SdpMaster *master)  = 0;

        virtual uint64_t getDiscoverTimeout(SdpMaster *master) = 0;
        virtual void onDiscoverTimeout(SdpMaster *master) = 0;
        
        virtual uint64_t getServiceTimeout(SdpMaster *master) = 0;
        virtual void onServiceTimeout(SdpMaster *master) = 0;

        virtual void onReceived(SdpMaster *master, const std::string &ip, uint16_t port, const std::string &buf) = 0;
    };

}

#endif // __SDP_MASTER_HANDLER_H__
