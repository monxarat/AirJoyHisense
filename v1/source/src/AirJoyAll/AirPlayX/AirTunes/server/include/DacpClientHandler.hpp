/*
* DacpClientQueryHandler.hpp
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

#ifndef __DACP_CLIENT_HANDLER_H__
#define __DACP_CLIENT_HANDLER_H__

#include "TcpClientMessage.hpp"
#include "AirJoyTypeDef.hpp"
#include "UmpClientHandler.hpp"
#include "HttpMessage.hpp"
#include "HttpMessageType.hpp"

namespace airjoy
{

	class DacpClient;
    class DacpClientListener;

    class DacpClientHandler : public UmpClientHandler
    {
    private:
		DacpClient			        *m_client;
        DacpClientListener          *m_listener;
        HttpMessagePointer           m_pHttpMessage;

    public:
        DacpClientHandler(DacpClient *client, DacpClientListener *listener);
        virtual ~DacpClientHandler();

        // UmpClientHandler
        virtual void didReceiveMessage(TcpClientMessagePointer p);
    };


}

#endif // __DACP_CLIENT_HANDLER_H__

