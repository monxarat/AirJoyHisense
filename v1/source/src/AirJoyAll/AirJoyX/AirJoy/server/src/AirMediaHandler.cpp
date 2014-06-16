/*
* AirMediaHandler.cpp
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

#include "AirMediaHandler.hpp"
#include "AirMediaListener.hpp"
#include "AirJoyDefault.hpp"
#include "ParamPhoto.hpp"
#include "ParamVideo.hpp"
#include "ParamAudio.hpp"
#include "ParamVolume.hpp"
#include "HttpTextParam.hpp"

using namespace airjoy;


AirMediaHandler::AirMediaHandler(AirMediaListener *listener)
    : m_listener(listener)
{
}

AirMediaHandler::~AirMediaHandler()
{
}

void AirMediaHandler::didSubscribeEvent(const std::string &fromIp, const std::string &topic)
{
	if (m_listener)
		m_listener->didSubscribeEvent(fromIp, topic);
}

void AirMediaHandler::didUnsubscribeEvent(const std::string &fromIp, const std::string &topic)
{
	if (m_listener)
		m_listener->didUnsubscribeEvent(fromIp, topic);
}

bool AirMediaHandler::doAction(const std::string &fromIp,
                               const std::string &topic, 
                               const std::string &action, 
                               const std::string &param, 
                               std::string &result)
{
    // ���Ż���������Բ���std::map

    // Action for photo
    if (action == AIRMEDIA_PHOTO_PUT)
        return doActionPhotoPut(fromIp, param, result);

    if (action == AIRMEDIA_PHOTO_DISPLAY)
        return doActionPhotoDisplay(fromIp, param, result);

    if (action == AIRMEDIA_PHOTO_STOP)
        return doActionPhotoStop(fromIp, param, result);

    if (action == AIRMEDIA_PHOTO_ROTATE)
        return doActionPhotoRotate(fromIp, param, result);

    if (action == AIRMEDIA_PHOTO_ZOOM)
        return doActionPhotoZoom(fromIp, param, result);
    
	if (action == AIRMEDIA_PHOTO_MOVE)
        return doActionPhotoMove(fromIp, param, result);

    // Action for video
    if (action == AIRMEDIA_VIDEO_PUT)
        return doActionVideoPut(fromIp, param, result);

    if (action == AIRMEDIA_VIDEO_RATE)
        return doActionVideoRate(fromIp, param, result);
 
    if (action == AIRMEDIA_VIDEO_STOP)
        return doActionVideoStop(fromIp, param, result);

    if (action == AIRMEDIA_VIDEO_SETPROGRESS)
        return doActionVideoSetProgress(fromIp, param, result);
    
    if (action == AIRMEDIA_VIDEO_GETPROGRESS)
        return doActionVideoGetProgress(fromIp, param, result);

    if (action == AIRMEDIA_VIDEO_GETPLAYBACKINFO)
        return doActionVideoGetPlaybackInfo(fromIp, param, result);

    // Action for audio
    if (action == AIRMEDIA_AUDIO_PUT)
        return doActionAudioPut(fromIp, param, result);
    
    if (action == AIRMEDIA_AUDIO_RATE)
        return doActionAudioRate(fromIp, param, result);
     
    if (action == AIRMEDIA_AUDIO_STOP)
        return doActionAudioStop(fromIp, param, result);

    if (action == AIRMEDIA_AUDIO_SETPROGRESS)
        return doActionAudioSetProgress(fromIp, param, result);
    
    if (action == AIRMEDIA_AUDIO_GETPROGRESS)
        return doActionAudioGetProgress(fromIp, param, result);
     
    if (action == AIRMEDIA_AUDIO_GETPLAYBACKINFO)
        return doActionAudioGetPlaybackInfo(fromIp, param, result);

    // Action for volume
    if (action == AIRMEDIA_VOLUME_SET)
        return doActionVolumeSet(fromIp, param, result);
    
    if (action == AIRMEDIA_VOLUME_GET)
        return doActionVolumeGet(fromIp, param, result);

	// Action for User defined
	return doActionUserDefined(fromIp, action, param, result);
}

// ������Ƭ��Ϣ
bool AirMediaHandler::doActionPhotoPut(const std::string &fromIp, const std::string &param, std::string &result)
{
    // ��������
    ParamPhoto photo;
    if (! photo.load(param, fromIp))
        return false;

    // ֪ͨӦ�ò�
    ResultCode code = ResultCodeError;
    if (photo.action() == PhotoActionDisplay)
        code = m_listener->displayPhoto(photo.photoId(), photo.url(), photo.name(), fromIp);
    else
        code = m_listener->cachePhoto(photo.photoId(), photo.url(), photo.name(), fromIp);

    // �ظ����
    std::stringstream ssResultCode;
    ssResultCode << code;
    HttpTextParam text;
    text.setValueByKey("code", ssResultCode.str());
    
    result = text.toString();

    return true;
}
  
bool AirMediaHandler::doActionPhotoDisplay(const std::string &fromIp, const std::string &param, std::string &result)
{
    // ��������
    ParamPhoto photo;
    if (! photo.load(param, fromIp))
        return false;

    // ֪ͨӦ�ò�
    ResultCode code = m_listener->displayCachedPhoto(photo.photoId(), fromIp);

    // �ظ����
    std::stringstream ssResultCode;
    ssResultCode << code;
    HttpTextParam text;
    text.setValueByKey("code", ssResultCode.str());
    result = text.toString();

    return true;
}

bool AirMediaHandler::doActionPhotoStop(const std::string &fromIp, const std::string &param, std::string &result)
{
    // ��������
    ParamPhoto photo;
    if (! photo.load(param, fromIp))
        return false;

    // ֪ͨӦ�ò�
    ResultCode code = m_listener->stopPhoto(fromIp);

    // �ظ����
    std::stringstream ssResultCode;
    ssResultCode << code;
    HttpTextParam text;
    text.setValueByKey("code", ssResultCode.str());
    result = text.toString();

    return true;
}

bool AirMediaHandler::doActionPhotoRotate(const std::string &fromIp, const std::string &param, std::string &result)
{
    // ��������
    ParamPhoto photo;
    if (! photo.load(param, fromIp))
        return false;

    // ֪ͨӦ�ò�
    ResultCode code = m_listener->rotatePhoto(photo.photoId(), photo.direction(), fromIp);

    // �ظ����
    std::stringstream ssResultCode;
    ssResultCode << code;
    HttpTextParam text;
    text.setValueByKey("code", ssResultCode.str());
    result = text.toString();
 
    return true;
}

bool AirMediaHandler::doActionPhotoZoom(const std::string &fromIp, const std::string &param, std::string &result)
{
    // ��������
    ParamPhoto photo;
    if (! photo.load(param, fromIp))
        return false;

    // ֪ͨӦ�ò�
    ResultCode code = m_listener->zoomPhoto(photo.photoId(), photo.rate(), fromIp);
    
    // �ظ����
    std::stringstream ssResultCode;
    ssResultCode << code;
    HttpTextParam text;
    text.setValueByKey("code", ssResultCode.str());
    result = text.toString();

    return true;
}

bool AirMediaHandler::doActionPhotoMove(const std::string &fromIp, const std::string &param, std::string &result)
{
    // ��������
    ParamPhoto photo;
    if (! photo.load(param, fromIp))
        return false;

    // ֪ͨӦ�ò�
	ResultCode code = m_listener->movePhoto(photo.photoId(), photo.x(), photo.y(), fromIp);
    
    // �ظ����
    std::stringstream ssResultCode;
    ssResultCode << code;
    HttpTextParam text;
    text.setValueByKey("code", ssResultCode.str());
    result = text.toString();

    return true;
}

// ������Ƶ��Ϣ
bool AirMediaHandler::doActionVideoPut(const std::string &fromIp, const std::string &param, std::string &result)
{
    // ��������
    ParamVideo video;
    if (! video.load(param, fromIp))
        return false;

    // ֪ͨӦ�ò�
    ResultCode code = m_listener->playVideo(video.videoId(), video.url(), video.name(), video.position(), fromIp);
    
    // �ظ����
    std::stringstream ssResultCode;
    ssResultCode << code;
    HttpTextParam text;
    text.setValueByKey("code", ssResultCode.str());
    result = text.toString();

    return true;
}

bool AirMediaHandler::doActionVideoRate(const std::string &fromIp, const std::string &param, std::string &result)
{
    // ��������
    ParamVideo video;
    if (! video.load(param, fromIp))
        return false;

    // ֪ͨӦ�ò�
    ResultCode code = m_listener->setPlayVideoRate(video.videoId(), video.rate(), fromIp);
    
    // �ظ����
    std::stringstream ssResultCode;
    ssResultCode << code;
    HttpTextParam text;
    text.setValueByKey("code", ssResultCode.str());
    result = text.toString();

    return true;
}

bool AirMediaHandler::doActionVideoStop(const std::string &fromIp, const std::string &param, std::string &result)
{
    // ��������
    ParamVideo video;
    if (! video.load(param, fromIp))
        return false;

    // ֪ͨӦ�ò�
    ResultCode code = m_listener->stopPlayVideo(video.videoId(), fromIp);
    
    // �ظ����
    std::stringstream ssResultCode;
    ssResultCode << code;
    HttpTextParam text;
    text.setValueByKey("code", ssResultCode.str());
    result = text.toString();

    return true;
}

bool AirMediaHandler::doActionVideoSetProgress(const std::string &fromIp, const std::string &param, std::string &result)
{
    // ��������
    ParamVideo video;
    if (! video.load(param, fromIp))
        return false;

    // ���ò��Ž���
    ResultCode code = m_listener->setPlayVideoProgress(video.videoId(), video.position(), fromIp);
    
    // �ظ����
    std::stringstream ssResultCode;
    ssResultCode << code;
    HttpTextParam text;
    text.setValueByKey("code", ssResultCode.str());
    result = text.toString();

    return true;
}

bool AirMediaHandler::doActionVideoGetProgress(const std::string &fromIp, const std::string &param, std::string &result)
{
    // ��������
    ParamVideo video;
    if (! video.load(param, fromIp))
        return false;

    float rate = 0.0f;
    float position = 0.0f;
    ResultCode code = m_listener->getPlayVideoProgress(video.videoId(), &rate, &position, fromIp);

    // ���ͻظ���Ϣ
    std::stringstream ssCode;
    std::stringstream ssRate;
    std::stringstream ssPosition;
    ssCode << code;
    ssRate << rate;
    ssPosition << position;

    HttpTextParam text;
    text.setValueByKey("code", ssCode.str());
    text.setValueByKey("id", video.videoId());
    text.setValueByKey("rate", ssRate.str());
    text.setValueByKey("position", ssPosition.str());
    result = text.toString();

    return true;
}

bool AirMediaHandler::doActionVideoGetPlaybackInfo(const std::string &fromIp, const std::string &param, std::string &result)
{
    // ��������
    ParamVideo video;
    if (! video.load(param, fromIp))
        return false;

    AirJoyPlayBackInfo info;
	info.rate = 0.0;
	info.position = 0.0;
	info.duration = 0.0;
    ResultCode code = m_listener->getPlayVideoInfo(info, fromIp);

    // ���ͻظ���Ϣ
    std::stringstream ssCode;
    std::stringstream ssRate;
    std::stringstream ssPosition;
    std::stringstream ssDuration;

    ssCode << code;
    ssRate << info.rate;
    ssPosition << info.position;
    ssDuration << info.duration;

    HttpTextParam text;
    text.setValueByKey("code", ssCode.str());
    text.setValueByKey("id", info.id);
    text.setValueByKey("url", info.url);
    text.setValueByKey("name", info.name);
    text.setValueByKey("rate", ssRate.str());
    text.setValueByKey("position", ssPosition.str());
    text.setValueByKey("duration", ssDuration.str());

    result = text.toString();

    return true;
}

// ������Ƶ��Ϣ
bool AirMediaHandler::doActionAudioPut(const std::string &fromIp, const std::string &param, std::string &result)
{
    // ��������
    ParamAudio audio;
    if (! audio.load(param, fromIp))
        return false;

    // ֪ͨӦ�ò�
	ResultCode code = m_listener->playAudio(audio.audioId(), audio.url(), audio.name(), audio.position(), fromIp);

    // �ظ����
    std::stringstream ssResultCode;
    ssResultCode << code;
    HttpTextParam text;
    text.setValueByKey("code", ssResultCode.str());
    result = text.toString();

    return true;
}

bool AirMediaHandler::doActionAudioRate(const std::string &fromIp, const std::string &param, std::string &result)
{
    // ��������
    ParamAudio audio;
    if (! audio.load(param, fromIp))
        return false;

    // ֪ͨӦ�ò�
    ResultCode code = m_listener->setPlayAudioRate(audio.audioId(), audio.rate(), fromIp);

    // �ظ����
    std::stringstream ssResultCode;
    ssResultCode << code;
    HttpTextParam text;
    text.setValueByKey("code", ssResultCode.str());
    result = text.toString();

    return true;
}

bool AirMediaHandler::doActionAudioStop(const std::string &fromIp, const std::string &param, std::string &result)
{
    // ��������
    ParamAudio audio;
    if (! audio.load(param, fromIp))
        return false;

    // ֪ͨӦ�ò�
    ResultCode code = m_listener->stopPlayAudio(audio.audioId(), fromIp);
    
    // �ظ����
    std::stringstream ssResultCode;
    ssResultCode << code;
    HttpTextParam text;
    text.setValueByKey("code", ssResultCode.str());
    result = text.toString();

    return true;
}

bool AirMediaHandler::doActionAudioSetProgress(const std::string &fromIp, const std::string &param, std::string &result)
{
    // ��������
    ParamAudio audio;
    if (! audio.load(param, fromIp))
        return false;

    // ���ò��Ž���
    ResultCode code = m_listener->setPlayAudioProgress(audio.audioId(), audio.position(), fromIp);
    
    // �ظ����
    std::stringstream ssResultCode;
    ssResultCode << code;
    HttpTextParam text;
    text.setValueByKey("code", ssResultCode.str());
    result = text.toString();

    return true;
}

bool AirMediaHandler::doActionAudioGetProgress(const std::string &fromIp, const std::string &param, std::string &result)
{
    // ��������
    ParamAudio audio;
    if (! audio.load(param, fromIp))
        return false;

    float rate = 0.0f;
    float position = 0.0f;

    ResultCode code = m_listener->getPlayAudioProgress(audio.audioId(), &rate, &position, fromIp);

    // ���ͻظ���Ϣ
    std::stringstream ssCode;
    std::stringstream ssRate;
    std::stringstream ssPosition;
    ssCode << code;
    ssRate << rate;
    ssPosition << position;

    // �ظ����
    std::stringstream ssResultCode;
    ssResultCode << code;
    HttpTextParam text;
    text.setValueByKey("code", ssCode.str());
    text.setValueByKey("id", audio.audioId());
    text.setValueByKey("rate", ssRate.str());
    text.setValueByKey("position", ssPosition.str());

    result = text.toString();

    return true;
}

bool AirMediaHandler::doActionAudioGetPlaybackInfo(const std::string &fromIp, const std::string &param, std::string &result)
{
    // ��������
    ParamAudio audio;
    if (! audio.load(param, fromIp))
        return false;

    AirJoyPlayBackInfo info;
	info.rate = 0.0;
	info.position = 0.0;
	info.duration = 0.0;
    ResultCode code = m_listener->getPlayAudioInfo(info, fromIp);

    // ���ͻظ���Ϣ
    std::stringstream ssCode;
    std::stringstream ssRate;
    std::stringstream ssPosition;
    std::stringstream ssDuration;
    
    ssCode << code;
    ssRate << info.rate;
    ssPosition << info.position;
    ssDuration << info.duration;

    HttpTextParam text;
    text.setValueByKey("code", ssCode.str());
    text.setValueByKey("id", info.id);
    text.setValueByKey("url", info.url);
    text.setValueByKey("name", info.name);
    text.setValueByKey("rate", ssRate.str());
    text.setValueByKey("position", ssPosition.str());
    text.setValueByKey("duration", ssDuration.str());

    result = text.toString();

    return true;
}

// ����������Ϣ
bool AirMediaHandler::doActionVolumeSet(const std::string &fromIp, const std::string &param, std::string &result)
{
    // ��������
    ParamVolume volume;
    if (! volume.load(param))
        return false;

    // ��������
    ResultCode code = m_listener->setVolume(volume.value(), fromIp);
    
    // �ظ����
    std::stringstream ssResultCode;
    ssResultCode << code;
    HttpTextParam text;
    text.setValueByKey("code", ssResultCode.str());
    result = text.toString();

    return true;
}

bool AirMediaHandler::doActionVolumeGet(const std::string &fromIp, const std::string &param, std::string &result)
{
    // ��������
    ParamVolume volume;
    if (! volume.load(param))
        return false;

    // ��ȡ����
    int value = 0;
    ResultCode code = m_listener->getVolume(&value, fromIp);

    // ���ͻظ���Ϣ
    std::stringstream ssCode;
    std::stringstream ssValue;
    ssCode << code;
    ssValue << value;

    // �ظ����
    std::stringstream ssResultCode;
    ssResultCode << code;
    HttpTextParam text;
    text.setValueByKey("code", ssCode.str());
    text.setValueByKey("volume", ssValue.str());
    result = text.toString();

    return true;
}

bool AirMediaHandler::doActionUserDefined(const std::string &fromIp, const std::string &action, const std::string &param, std::string &result)
{
    char r[1024];
    size_t len = sizeof(r);
    memset(r, 0,len);

	bool ret = m_listener->didReceiveRequest(action, param, r, len - 1, fromIp);

    result.append(r);

    return ret;
}
