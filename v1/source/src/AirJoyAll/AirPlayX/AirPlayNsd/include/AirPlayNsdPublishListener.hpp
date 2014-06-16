/*
* AirPlayNsdPublishListener.hpp
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

#ifndef __AIRPLAY_NSD_PUBLISH_LISTENER_H__
#define __AIRPLAY_NSD_PUBLISH_LISTENER_H__

#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"

#include "AirPlayServiceInfo.hpp"

namespace airjoy
{
    
    /**
     * AirPlay������񷢲�������
     */

    class AIRJOY_API AirPlayNsdPublishListener
    {
    public:
        AirPlayNsdPublishListener() {}
        virtual ~AirPlayNsdPublishListener() {}

        /**
         * �������񣺳ɹ�
         *  @serviceType    [in] ��������
         *  @serviceName    [in] ��������
         */
        virtual void onServicePublished(const std::string & servicetype, const std::string & serviceName) = 0;

        /**
         * ��������ʧ��
         *  @serviceType    [in] ��������
         *  @serviceName    [in] ��������
         *  @errorCode      [in] �������
         */
        virtual void onPublishFailed(const std::string & servicetype, const std::string & serviceName, int errorCode) = 0;

        /**
         * ȡ���������񣺳ɹ�
         *  @serviceType    [in] ��������
         *  @serviceName    [in] ��������
         *  @errorCode      [in] �������
         */
        virtual void onServiceUnPublished(const std::string & servicetype, const std::string & serviceName) = 0;
        
        /**
         * ȡ����������ʧ��
         *  @serviceType    [in] ��������
         *  @serviceName    [in] ��������
         *  @errorCode      [in] �������
         */
        virtual void onUnPulishFailed(const std::string & servicetype, const std::string & serviceName, int errorCode) = 0;
    };

}



#endif // __AIRPLAY_NSD_PUBLISH_LISTENER_H__

