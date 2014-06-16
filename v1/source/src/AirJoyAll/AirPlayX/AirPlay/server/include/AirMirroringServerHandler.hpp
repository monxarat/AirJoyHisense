/*
* AirMirroringServerHandler.hpp
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
*
*/

#ifndef __AIR_MIRRORING_SERVER_HANDLER_H__
#define __AIR_MIRRORING_SERVER_HANDLER_H__

#include "TcpServerMessage.hpp"
#include "AirJoyTypeDef.hpp"
#include "UmpServerHandler.hpp"
#include "UmpMessage.hpp"
#include "HttpMessage.hpp"
#include "AirMirroringServer.hpp"
#include <map>

namespace airjoy
{

    class AirMirroringServerListener;

    class AirMirroringServerHandler : public UmpServerHandler
    {
    public:
        typedef TcpServerMessage::TcpSessionPointer         TcpSessionPointer;
        typedef boost::shared_ptr<HttpMessage>              HttpMessagePointer;
        typedef std::map<int, TcpSessionPointer>            TcpSessionMap;
        typedef boost::mutex                                MutexType;
        typedef MutexType::scoped_lock                      LockType;

        typedef enum _AirMrroringStatus
        {
            AIR_MIRRORING_STATUS_UNKNOWN              = 0,
            AIR_MIRRORING_STATUS_OK                   = 200,
            AIR_MIRRORING_STATUS_SWITCHING_PROTOCOLS  = 101,
            AIR_MIRRORING_STATUS_NEED_AUTH            = 401,
            AIR_MIRRORING_STATUS_NOT_FOUND            = 404,
            AIR_MIRRORING_STATUS_METHOD_NOT_ALLOWED   = 405,
            AIR_MIRRORING_STATUS_NOT_IMPLEMENTED      = 501,
            AIR_MIRRORING_STATUS_NO_RESPONSE_NEEDED   = 1000,
        } AirMrroringStatus;

    private:
        AirMirroringServerListener      *m_listener;
        std::string                      m_time;

        // 一个会话，对应一个HTTP消息解析器
        typedef std::map<int, HttpMessagePointer> HttpParserMap;
        HttpParserMap                                       m_httpParsers;
        MutexType                                           m_httpParsersMutex;

    public:
        AirMirroringServerHandler(AirMirroringServer *server, AirMirroringServerListener *listener);
        virtual ~AirMirroringServerHandler();

        // UmpServerHandler
        virtual bool didReceiveMessage(TcpServerMessagePointer request);
        virtual void didClose(int connectedId);

    private:
        bool insertHttpParser(int connectedId);
        bool removeHttpParser(int connectedId);
        HttpMessagePointer getHttpParser(int connectedId);

        const std::string & getNowTime(void);
        void sendMessage(TcpSessionPointer session, const std::string & message);
        void sendResponseOk(TcpSessionPointer session, std::string &responseContent);
        void sendResponseSwitchingProtocols(TcpSessionPointer session);
        void sendResponseUnauthorized(TcpSessionPointer session);
        void sendResponseNotFound(TcpSessionPointer session);
        void sendResponseNotImplemented(TcpSessionPointer session);
        void sendResponseMethodNotAllowed(TcpSessionPointer session);

        AirMrroringStatus didGet(TcpServerMessagePointer request, HttpMessagePointer pHttpMessage, std::string &responseContent);
        AirMrroringStatus didPost(TcpServerMessagePointer request, HttpMessagePointer pHttpMessage, std::string &responseContent);

        AirMrroringStatus didGetServerInfo(TcpServerMessagePointer request, HttpMessagePointer pHttpMessage, std::string &responseContent);

        AirMrroringStatus didPostFpSetup(TcpServerMessagePointer request, HttpMessagePointer pHttpMessage, std::string &responseContent);
    };

}


#endif // __AIR_MIRRORING_SERVER_HANDLER_H__
