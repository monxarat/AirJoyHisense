/*
* AirJoyNsdPublishListener.hpp
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

#ifndef __AIRJOY_NSD_PUBLISH_LISTENER_H__
#define __AIRJOY_NSD_PUBLISH_LISTENER_H__

#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"
#include "AirJoyServiceInfo.hpp"

namespace airjoy
{

    class AIRJOY_API AirJoyNsdPublishListener
    {
    public:
        AirJoyNsdPublishListener() {}
        virtual ~AirJoyNsdPublishListener() {}
           
        /**
         * ��������ɹ�
         *  @serviceInfo    [in] ������Ϣ
         */
        virtual void onServicePublished(AirJoyServiceInfo & serviceInfo) = 0;

        /**
         * ��������ʧ��
         *  @serviceInfo    [in] ������Ϣ
         *  @errorCode      [in] �������
         */
        virtual void onPublishFailed(AirJoyServiceInfo & serviceInfo, int errorCode) = 0;

        /**
         * ȡ���������񣺳ɹ�
         *  @serviceInfo    [in] ������Ϣ
         *  @errorCode      [in] �������
         */
        virtual void onServiceUnPublished(AirJoyServiceInfo & serviceInfo) = 0;

        /**
         * ȡ����������ʧ��
         *  @serviceInfo    [in] ������Ϣ
         *  @errorCode      [in] �������
         */
        virtual void onUnPulishFailed(AirJoyServiceInfo & serviceInfo, int errorCode) = 0;
    };

}



#endif // __AIRJOY_NSD_PUBLISH_LISTENER_H__

