/*
* AirPortControlHandler.cpp
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

#include "AirPortControlHandler.hpp"
#include "UdpMessage.hpp"

using namespace airjoy;

AirPortControlHandler::AirPortControlHandler() 
{
}

AirPortControlHandler::~AirPortControlHandler()
{
}

// notify app, the udp server has been started;
void AirPortControlHandler::didStart() 
{
    //std::cout << "AirPortControlHandler::didStart" << std::endl;
}

// return true if this message can be handled, otherwise return false.
bool AirPortControlHandler::didReceiveMessage(UdpMessagePointer message) 
{ 
    //std::cout << "AirPortControlHandler::didReceiveMessage: " << message->remoteIp() << " : " << message->remotePort() << " len: " << message->message().length() << std::endl;
    //std::cout << message->message()  << std::endl;
    return true;
}

// notify app, the tcp session has been closed;
void AirPortControlHandler::didClose() 
{
    //std::cout << "AirPortControlHandler::didClose" << std::endl;
}