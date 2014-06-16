/*
* AirPortDataHandler.cpp
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

#include "AirPortDataHandler.hpp"
#include "UdpMessage.hpp"
#include "AudioBuffer.hpp"

#include <stdlib.h>

using namespace airjoy;

#define MAX_PACKET      2048

#ifdef _WIN32
#ifndef ssize_t
	typedef int ssize_t;
#endif
#endif // _WIN32

typedef unsigned short seq_t;

AirPortDataHandler::AirPortDataHandler() 
{
}

AirPortDataHandler::~AirPortDataHandler()
{
}

// notify app, the udp server has been started;
void AirPortDataHandler::didStart() 
{
    //std::cout << "AirPortDataHander::didStart" << std::endl;
}

// return true if this message can be handled, otherwise return false.
bool AirPortDataHandler::didReceiveMessage(UdpMessagePointer message) 
{
    //std::cout << "AirPortDataHandler::didReceiveMessage: " << message->remoteIp() << " : " << message->remotePort() << " len: " << message->message().size() << std::endl;
    
#if 0
    seq_t seqno = 0;
    ssize_t packetLen = 0;
    char *pPacket = NULL;
    char packet[MAX_PACKET];

    memset(packet, 0, MAX_PACKET);
    packetLen = message->message().length();
    memcpy(packet, message->message().c_str(), packetLen);

    char type = packet[1] & ~0x80;
    if (type == 0x60 || type == 0x56) 
    {
        // audio data / resend
        pPacket = packet;
        if (type == 0x56)
        {
            pPacket += 4;
            packetLen -= 4;
        }

        seqno = ntohs(*(unsigned short *)(pPacket + 2));
        m_audioBuffer->putPacket(seqno, pPacket + 12, packetLen - 12);
    }

    return true;
#else
    
    seq_t seqno = 0;
    ssize_t packetLen = 0;
    const char *pPacket = NULL;
    const char *packet = message->message().c_str();
    //const char *packet = message->buf();

    //memset(packet, 0, MAX_PACKET);
    packetLen = message->message().length();
    //packetLen = message->bufSize();
    //memcpy(packet, message->message().c_str(), packetLen);

    char type = packet[1] & ~0x80;
    if (type == 0x60 || type == 0x56) 
    {
        // audio data / resend
        pPacket = packet;
        if (type == 0x56)
        {
            pPacket += 4;
            packetLen -= 4;
        }

        seqno = ntohs(*(unsigned short *)(pPacket + 2));
        m_audioBuffer->putPacket(seqno, pPacket + 12, packetLen - 12);
    }

    return true;
#endif
}

// notify app, the tcp session has been closed;
void AirPortDataHandler::didClose() 
{
    //std::cout << "AirPortDataHandler::didClose" << std::endl;
}

void AirPortDataHandler::setBuffer(AudioBuffer *audioBuffer)
{
    m_audioBuffer = audioBuffer;
}
