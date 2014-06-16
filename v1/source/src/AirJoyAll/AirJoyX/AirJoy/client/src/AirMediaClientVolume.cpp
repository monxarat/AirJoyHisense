/*
 * AirMediaClientVolume.cpp
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

#include "AirMediaClientVolume.hpp"
#include "AirMediaClient.hpp"
#include "AirJoyDefault.hpp"
#include "HttpTextParam.hpp"
#include <sstream>

using namespace airjoy;


AirMediaClientVolume::AirMediaClientVolume(AirMediaClient *sender)
    : m_sender(sender)
{
}

AirMediaClientVolume::~AirMediaClientVolume()
{
}

AirJoySessionId AirMediaClientVolume::setVolume(int volume)
{
    std::stringstream ssValue;
    ssValue << volume;

    HttpTextParam param;
    param.setValueByKey("value", ssValue.str());

    return m_sender->send(AIRMEDIA_VOLUME_SET, param.toString());
}

AirJoySessionId AirMediaClientVolume::getVolume(void)
{       
    HttpTextParam param;

    return m_sender->send(AIRMEDIA_VOLUME_GET, param.toString());
}