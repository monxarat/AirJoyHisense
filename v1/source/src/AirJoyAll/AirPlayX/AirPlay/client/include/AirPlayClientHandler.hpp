/*
* AirPlayClientHandler.hpp
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

#ifndef __AIRPLAY_CLIENT_HANDLER_H__
#define __AIRPLAY_CLIENT_HANDLER_H__

#include "TcpClientMessage.hpp"
#include "AnyPlayTypeDef.hpp"
#include "UmpClientHandler.hpp"
#include "HttpMessage.hpp"

namespace anyplay
{

    class AirPlayClientListener;
    class AirPlayClient;

    class AirPlayClientHandler : public UmpClientHandler
    {
    private:
        AirPlayClientListener *m_listener;
        AirPlayClient         *m_client;

    public:
        AirPlayClientHandler(AirPlayClient *client, AirPlayClientListener *listener);
        virtual ~AirPlayClientHandler();

        // UmpClientHandler
        virtual void didReceiveMessage(TcpClientMessagePointer p);

    private:
        void didHandleResponse(HttpMessage &response);
    };


}

#endif // __AIRPLAY_CLIENT_HANDLER_H__
