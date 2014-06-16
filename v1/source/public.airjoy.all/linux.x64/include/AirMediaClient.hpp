/*
* AirMediaClient.hpp
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

#ifndef __AIRMEDIA_CLIENT_H__
#define __AIRMEDIA_CLIENT_H__

#include "AirJoyClient.hpp"

#include "AirMediaClientPhoto.hpp"
#include "AirMediaClientVideo.hpp"
#include "AirMediaClientAudio.hpp"
#include "AirMediaClientVolume.hpp"


namespace airjoy
{


    class AirMediaClientQueryListener;

    class AIRJOY_API AirMediaClient : public AirJoyClient
    {
    public:
        AirMediaClient();
        virtual ~AirMediaClient();
        
        /**
         * ���ü�����
         *  @listener     [in] ������
         */
        void setListener(AirMediaClientQueryListener *listener);

		/**
         * �����¼�
         *  @ip             [in] ����˵�ַ
         *  @port           [in] ����˿�
         *  @return
         *          - true ����ִ�гɹ�
         *          - false ����ִ��ʧ��
         */
        bool subscribeEvent(const std::string &ip, uint16_t port);
		
        /**
         * ȡ������
         *  @return
         *          - true ����ִ�гɹ�
         *          - false ����ִ��ʧ��
         */
        bool unsubscribeEvent(void);

        /**
         * �����Զ�����Ϣ
         *  @appAction       [in] Ӧ���Զ�����Ϊ
         *  @appParam        [in] Ӧ���Զ������
         *  @return �ỰID
         *  @note ֻ�ж���֮�󣬲��ܷ�����Ϣ
         */
		AirJoySessionId send(const std::string &appAction,
                             const std::string &appParam);

        /**
         * ��Ƭ����ģ��
         */
        AirMediaClientPhoto * photo(void);

        /**
         * ��Ƶ����ģ��
         */
        AirMediaClientVideo * video(void);
        
        /**
         * ��Ƶ����ģ��
         */
        AirMediaClientAudio * audio(void);

        /**
         * ϵͳ����ģ��
         */
        AirMediaClientVolume * volume(void);

    private:
        int						   m_sessionId;
		AirMediaClientPhoto      * m_photo;
        AirMediaClientVideo      * m_video;
        AirMediaClientAudio      * m_audio;
        AirMediaClientVolume     * m_volume;
    };


}

#endif // __AIRMEDIA_CLIENT_H__
