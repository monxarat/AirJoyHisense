
#ifndef __AJCLIENTEVENTLISTNER_H__
#define __AJCLIENTEVENTLISTNER_H__

#include "AirMediaClientEventListener.hpp"

class AJClientEventListener : public AirMediaClientEventListener
{
    public:
        AJClientEventListener();
        virtual ~AJClientEventListener();

        /**
         * 超时
         */
		virtual void didTimeout(void) ;
		
		/**
         * 网络错误
         */
		virtual void didErrorNet(void) ;

        /**
         * 订阅事件成功
         */
        virtual void didSubscribeEvent(void) ;

        /**
         * 取消订阅事件成功
         */
        virtual void didUnsubscribeEvent(void) ;

        /**
         * 收到事件
         *  @type           [in] 类型：photo/audio/video
         *  @action         [in] 行为：play/pause/stop
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

