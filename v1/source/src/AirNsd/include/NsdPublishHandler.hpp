/*
* NsdPublishHandler.hpp
*
* Service Discovery Protocol Delegate
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

#ifndef __NSD_PUBLISH_HANDLER_H__
#define __NSD_PUBLISH_HANDLER_H__

#include "TheBoost.hpp"

namespace airjoy
{

    class Nsd;
    class NsdServiceInfo;

    class NsdPublishHandler
    {
    public:
        typedef boost::shared_ptr<NsdServiceInfo>   NsdServiceInfoPointer;

    public:
        NsdPublishHandler() {}
        ~NsdPublishHandler() {}

        virtual void didNotPublish(NsdServiceInfoPointer serviceInfo) = 0;
        virtual void willPublish(NsdServiceInfoPointer serviceInfo) = 0;
        virtual void didPublish(NsdServiceInfoPointer serviceInfo) = 0;
    };

}

#endif // __NSD_PUBLISH_HANDLER_H__
