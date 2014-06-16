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
         * 设置监听者
         *  @listener     [in] 监听者
         */
        void setListener(AirMediaClientQueryListener *listener);

		/**
         * 订阅事件
         *  @ip             [in] 服务端地址
         *  @port           [in] 服务端口
         *  @return
         *          - true 函数执行成功
         *          - false 函数执行失败
         */
        bool subscribeEvent(const std::string &ip, uint16_t port);
		
        /**
         * 取消订阅
         *  @return
         *          - true 函数执行成功
         *          - false 函数执行失败
         */
        bool unsubscribeEvent(void);

        /**
         * 发送自定义消息
         *  @appAction       [in] 应用自定义行为
         *  @appParam        [in] 应用自定义参数
         *  @return 会话ID
         *  @note 只有订阅之后，才能发送消息
         */
		AirJoySessionId send(const std::string &appAction,
                             const std::string &appParam);

        /**
         * 照片功能模块
         */
        AirMediaClientPhoto * photo(void);

        /**
         * 视频功能模块
         */
        AirMediaClientVideo * video(void);
        
        /**
         * 音频功能模块
         */
        AirMediaClientAudio * audio(void);

        /**
         * 系统功能模块
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
