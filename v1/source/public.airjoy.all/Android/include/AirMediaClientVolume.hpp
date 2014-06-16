/*
 * AirMediaClientVolume.hpp
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

#ifndef __AIRMEDIA_CLIENT_VOLUME_H__
#define __AIRMEDIA_CLIENT_VOLUME_H__


#include <string>
#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"

namespace airjoy
{

    class AirMediaClient;

    class AIRJOY_API AirMediaClientVolume
    {
    public:
        AirMediaClientVolume(AirMediaClient *sender);
        virtual ~AirMediaClientVolume();

        /**
         * 设置音量
         *  @volume         [in] 当前音量(1-100)
         *  @return 会话ID
         */
        AirJoySessionId    setVolume                      (int volume);

        /**
         * 获取音量
         *  @return 会话ID
         */
        AirJoySessionId    getVolume                      (void);

    private:
        AirMediaClient  *m_sender;
    };

}


#endif // __AIRMEDIA_CLIENT_VOLUME_H__
