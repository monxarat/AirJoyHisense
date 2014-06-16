/*
 * AirMediaClientAudio.cpp
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

#include "AirMediaClientAudio.hpp"
#include "AirMediaClient.hpp"
#include "AirJoyDefault.hpp"
#include "HttpTextParam.hpp"
#include <sstream>

using namespace airjoy;


AirMediaClientAudio::AirMediaClientAudio(AirMediaClient *sender)
    : m_sender(sender)
{
}

AirMediaClientAudio::~AirMediaClientAudio()
{
}

AirJoySessionId AirMediaClientAudio::playAudio(const std::string &audioUrl,
                                               const std::string &audioId,
                                               const std::string &audioNameUtf8,
                                               float startPosition)
{
    std::stringstream ssStartPosition;
    ssStartPosition << startPosition;

    HttpTextParam param;
    param.setValueByKey("id", audioId);
    param.setValueByKey("url", audioUrl);
    param.setValueByKey("name", audioNameUtf8);
    param.setValueByKey("position", ssStartPosition.str());

    return m_sender->send(AIRMEDIA_AUDIO_PUT, param.toString());
}

AirJoySessionId AirMediaClientAudio::setPlayAudioRate(const std::string &audioId,
                                                      float rate)
{
    std::stringstream ssRate;
    ssRate << rate;

    HttpTextParam param;
    param.setValueByKey("id", audioId);
    param.setValueByKey("rate", ssRate.str());

    return m_sender->send(AIRMEDIA_AUDIO_RATE, param.toString());
}

AirJoySessionId AirMediaClientAudio::stopPlayAudio(const std::string &audioId)
{
    HttpTextParam param;
    param.setValueByKey("id", audioId);

    return m_sender->send(AIRMEDIA_AUDIO_STOP, param.toString());
}

AirJoySessionId AirMediaClientAudio::setPlayAudioProgress(const std::string &audioId,
                                                          float playPosition)
{
    std::stringstream ssPlayPosition;
    ssPlayPosition << playPosition;

    HttpTextParam param;
    param.setValueByKey("id", audioId);
    param.setValueByKey("position", ssPlayPosition.str());

    return m_sender->send(AIRMEDIA_AUDIO_SETPROGRESS, param.toString());
}

AirJoySessionId AirMediaClientAudio::getPlayAudioProgress(const std::string &audioId)
{
    HttpTextParam param;
    param.setValueByKey("id", audioId);

    return m_sender->send(AIRMEDIA_AUDIO_GETPROGRESS, param.toString());
}

AirJoySessionId AirMediaClientAudio::getPlayAudioInfo(void)
{
    HttpTextParam param;

    return m_sender->send(AIRMEDIA_AUDIO_GETPLAYBACKINFO, param.toString());
}