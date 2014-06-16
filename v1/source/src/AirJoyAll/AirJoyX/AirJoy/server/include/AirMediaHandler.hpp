/*
* AirMediaHandler.hpp
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

#ifndef __AIR_MEDIA_HANDLER_H__
#define __AIR_MEDIA_HANDLER_H__

#include "AirMessageHandler.hpp"

namespace airjoy
{

    class AirMediaListener;

    class AirMediaHandler : public AirMessageHandler
    {
    public:
        AirMediaHandler(AirMediaListener *listener);
        virtual ~AirMediaHandler();

        // AirMessageHandler
        virtual bool doAction(const std::string &fromIp,
                              const std::string &topic, 
                              const std::string &action, 
                              const std::string &param, 
							  std::string &result);
        virtual void didSubscribeEvent(const std::string &fromIp, const std::string &topic);        
        virtual void didUnsubscribeEvent(const std::string &fromIp, const std::string &topic);
    
	private:
        // ������Ƭ��Ϣ
        bool doActionPhotoPut(const std::string &fromIp, const std::string &param, std::string &result);
        bool doActionPhotoDisplay(const std::string &fromIp, const std::string &param, std::string &result);
        bool doActionPhotoStop(const std::string &fromIp, const std::string &param, std::string &result);
        bool doActionPhotoRotate(const std::string &fromIp, const std::string &param, std::string &result);
        bool doActionPhotoZoom(const std::string &fromIp, const std::string &param, std::string &result);
		bool doActionPhotoMove(const std::string &fromIp, const std::string &param, std::string &result);

        // ������Ƶ��Ϣ
        bool doActionVideoPut(const std::string &fromIp, const std::string &param, std::string &result);
        bool doActionVideoRate(const std::string &fromIp, const std::string &param, std::string &result);
        bool doActionVideoStop(const std::string &fromIp, const std::string &param, std::string &result);
        bool doActionVideoSetProgress(const std::string &fromIp, const std::string &param, std::string &result);
        bool doActionVideoGetProgress(const std::string &fromIp, const std::string &param, std::string &result);
        bool doActionVideoGetPlaybackInfo(const std::string &fromIp, const std::string &param, std::string &result);

        // ������Ƶ��Ϣ
        bool doActionAudioPut(const std::string &fromIp, const std::string &param, std::string &result);
        bool doActionAudioRate(const std::string &fromIp, const std::string &param, std::string &result);
        bool doActionAudioStop(const std::string &fromIp, const std::string &param, std::string &result);
        bool doActionAudioSetProgress(const std::string &fromIp, const std::string &param, std::string &result);
        bool doActionAudioGetProgress(const std::string &fromIp, const std::string &param, std::string &result);
        bool doActionAudioGetPlaybackInfo(const std::string &fromIp, const std::string &param, std::string &result);

        // ����������Ϣ
        bool doActionVolumeSet(const std::string &fromIp, const std::string &param, std::string &result);
        bool doActionVolumeGet(const std::string &fromIp, const std::string &param, std::string &result);

		// �û��Զ�����Ϣ
		bool doActionUserDefined(const std::string &fromIp, const std::string &action, const std::string &param, std::string &result);

    private:
        AirMediaListener    *m_listener;
    };

}

#endif // __AIR_MEDIA_HANDLER_H__

