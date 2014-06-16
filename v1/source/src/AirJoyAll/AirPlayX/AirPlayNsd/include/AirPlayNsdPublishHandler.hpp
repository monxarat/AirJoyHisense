/*
* AirPlayNsdPublishHandler.hpp
*
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

#ifndef __AIRPLAY_NSD_PUBLISH_HANDLER_H__
#define __AIRPLAY_NSD_PUBLISH_HANDLER_H__

#include "AirPlayServiceInfo.hpp"
#include "NsdPublishHandler.hpp"

namespace airjoy
{

    class AirPlayNsdPublishListener;

    class AirPlayNsdPublishHandler : public NsdPublishHandler
    {
    private:
        AirPlayNsdPublishListener   *m_listener;

    public:
        AirPlayNsdPublishHandler(AirPlayNsdPublishListener * listener);
        virtual ~AirPlayNsdPublishHandler();

        virtual void didNotPublish(NsdServiceInfoPointer serviceInfo);
        virtual void willPublish(NsdServiceInfoPointer serviceInfo);
        virtual void didPublish(NsdServiceInfoPointer serviceInfo);
    };

}



#endif // __AIRPLAY_NSD_PUBLISH_HANDLER_H__
