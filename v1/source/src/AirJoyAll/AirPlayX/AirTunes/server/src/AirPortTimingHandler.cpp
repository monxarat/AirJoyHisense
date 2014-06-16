/*
* AirPortTimingHandler.cpp
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

#include "AirPortTimingHandler.hpp"
#include "UdpMessage.hpp"

using namespace airjoy;

AirPortTimingHandler::AirPortTimingHandler() 
{
}

AirPortTimingHandler::~AirPortTimingHandler()
{
}

void AirPortTimingHandler::didStart() 
{
    //std::cout << "AirPortTimingHandler::didStart" << std::endl;
}

bool AirPortTimingHandler::didReceiveMessage(UdpMessagePointer message) 
{ 
    //std::cout << "AirPortTimingHandler::didReceiveMessage: " << message->remoteIp() << " : " << message->remotePort() << " len: " << message->bufSize() << std::endl;
    //std::cout << message->message()  << std::endl;
    return true;
}

void AirPortTimingHandler::didClose() 
{
    //std::cout << "AirPortTimingHandler::didClose" << std::endl;
}