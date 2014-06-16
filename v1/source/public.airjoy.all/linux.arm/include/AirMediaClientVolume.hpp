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
         * ��������
         *  @volume         [in] ��ǰ����(1-100)
         *  @return �ỰID
         */
        AirJoySessionId    setVolume                      (int volume);

        /**
         * ��ȡ����
         *  @return �ỰID
         */
        AirJoySessionId    getVolume                      (void);

    private:
        AirMediaClient  *m_sender;
    };

}


#endif // __AIRMEDIA_CLIENT_VOLUME_H__
