/*
* AirMediaClientQueryHandler.hpp
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

#ifndef __AIRMEDIA_CLIENT_QUERY_HANDLER_H__
#define __AIRMEDIA_CLIENT_QUERY_HANDLER_H__

#include "TcpClientMessage.hpp"
#include "AirJoyTypeDef.hpp"
#include "UmpClientHandler.hpp"
#include "HttpMessage.hpp"
#include "HttpMessageType.hpp"

namespace airjoy
{

	class AirMediaClient;
    class AirMediaClientQueryListener;

    class AirMediaClientQueryHandler : public UmpClientHandler
    {
    public:
        typedef boost::mutex                                MutexType;
        typedef MutexType::scoped_lock                      LockType;

    private:
		AirMediaClient			        *m_client;
        AirMediaClientQueryListener     *m_listener;
        HttpMessagePointer               m_pHttpMessage;
        
        // »º³å
        std::string                      m_buffer;
        MutexType                        m_bufferMutex;

    public:
        AirMediaClientQueryHandler(AirMediaClient *client, AirMediaClientQueryListener *listener);
        virtual ~AirMediaClientQueryHandler();

        // UmpClientHandler
        virtual void didReceiveMessage(TcpClientMessagePointer p);

    private:
        void addBuffer(TcpClientMessagePointer p);
        bool handleBuffer(const std::string &serverIp);

        int getSessionId(const std::string &message);
        int getQuerySessionId(const std::string &stringSessionId);

        void didHandleHttpResponse(const std::string &serverIp, HttpMessagePointer pHttpResponse);
        void didHandleHttpRequest(const std::string &serverIp, HttpMessagePointer pHttpRequest);

        void didHandlePubsubResult(const std::string &serverIp, const std::string &action, const std::string &param);
        void didHandleQueryResult(int sessionId, const std::string &action, const std::string &param);
    };


}

#endif // __AIRMEDIA_CLIENT_QUERY_HANDLER_H__
