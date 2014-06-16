
#ifndef __AJCLIENTEVENTLISTNER_H__
#define __AJCLIENTEVENTLISTNER_H__

#include "AirMediaClientEventListener.hpp"

class AJClientEventListener : public AirMediaClientEventListener
{
    public:
        AJClientEventListener();
        virtual ~AJClientEventListener();

        /**
         * ��ʱ
         */
		virtual void didTimeout(void) ;
		
		/**
         * �������
         */
		virtual void didErrorNet(void) ;

        /**
         * �����¼��ɹ�
         */
        virtual void didSubscribeEvent(void) ;

        /**
         * ȡ�������¼��ɹ�
         */
        virtual void didUnsubscribeEvent(void) ;

        /**
         * �յ��¼�
         *  @type           [in] ���ͣ�photo/audio/video
         *  @action         [in] ��Ϊ��play/pause/stop
         *  @id             [in] id for photo/audio/video
         *  @url            [in, option] url for photo/audio/video
         *  @name           [in, option] name for photo/audio/video
         */
        virtual void recvEvent(const std::string &type,
                               const std::string &action,
                               const std::string &id,
                               const std::string &url,
                               const std::string &name) ;

};
#endif

