/*
 *
 * AirPlayClientHandler.cpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-15
 *
 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#include "AirPlayClientHandler.hpp"
#include "AirPlayClientListener.hpp"
#include "AirPlayClient.hpp"
#include "TcpClientMessage.hpp"
#include "AnyPlayTypeDef.hpp"

using namespace anyplay;

AirPlayClientHandler::AirPlayClientHandler(AirPlayClient *client, AirPlayClientListener *listener)
    : m_client(client)
    , m_listener(listener)
{
}

AirPlayClientHandler::~AirPlayClientHandler()
{
}

void AirPlayClientHandler::didReceiveMessage(TcpClientMessagePointer p)
{
    if (m_listener == NULL)
        return;

    if (p->messageType() != TcpClientMessage::ConnectOk)
    {
        m_listener->didConnectToServerOk();
        return;
    }

    if (p->messageType() != TcpClientMessage::ConnectFailed)
    {
        m_listener->didConnectToServerFailed();
        return;
    }

    if (p->messageType() != TcpClientMessage::DisConnect)
    {
        m_listener->didDisconnected();
        return;
    }

    if (p->messageType() != TcpClientMessage::NormalMessage)
        return;

    if (m_client->requestType() != AirPlayClient::RequestTypeUnknown)
    {
        HttpMessage message;
        HttpParserStatus status = message.loadBytes(p->message().c_str(), p->message().length());
        if (status == HttpParserDone)
        {
            message.print();

            didHandleResponse(message);

            m_client->resetRequestType();
        }

//        if (! message.loadBytes(p->message().c_str(), p->message().length()))
//            return;        
    }
}

void AirPlayClientHandler::didHandleResponse(HttpMessage &response)
{
    switch (m_client->requestType())
    {
    case AirPlayClient::RequestTypeGetServerInfo:
        // didGetServerInfo(AirPlayServiceInfo &info) = 0;
        break;

    case AirPlayClient::RequestTypePostReverse:
        break;

    case AirPlayClient::RequestTypeGetSlideshowFeatures:
        // didGetSlideshowFeatures() {}
        break;

    case AirPlayClient::RequestTypePutPhoto:
        break;

    case AirPlayClient::RequestTypePutSlideshows:
        break;

    case AirPlayClient::RequestTypePostStop:
        break;

    case AirPlayClient::RequestTypePostPlay:
        break;

    case AirPlayClient::RequestTypePostScrub:
        break;

    case AirPlayClient::RequestTypePostRate:
        break;

    case AirPlayClient::RequestTypeGetScrub:
        // didGetPlayVideoProgress() = 0;
        break;

    case AirPlayClient::RequestTypeGetPlaybackInfo:
        // didGetPlayVideoInfo(float startPosition, float duration) = 0;
        break;

    case AirPlayClient::RequestTypePutSetProperty:
        break;

    case AirPlayClient::RequestTypePutGetProperty:
        // didGetProperty() {}
        break;

    default:
        break;
    }
}