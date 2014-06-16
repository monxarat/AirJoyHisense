
#ifndef __AJPUBLISHLISTENER_H__
#define __AJPUBLISHLISTENER_H__


#include "AirJoyNsdPublishListener.hpp"


using namespace airjoy;

class AJPublishListener: public AirJoyNsdPublishListener
{
	public:
		 /**
         * ��������ɹ�
         *  @serviceInfo    [in] ������Ϣ
         */
        virtual void onServicePublished(AirJoyServiceInfo & serviceInfo);

        /**
         * ��������ʧ��
         *  @serviceInfo    [in] ������Ϣ
         *  @errorCode      [in] �������
         */
        virtual void onPublishFailed(AirJoyServiceInfo & serviceInfo, int errorCode);

        /**
         * ȡ���������񣺳ɹ�
         *  @serviceInfo    [in] ������Ϣ
         *  @errorCode      [in] �������
         */
        virtual void onServiceUnPublished(AirJoyServiceInfo & serviceInfo);

        /**
         * ȡ����������ʧ��
         *  @serviceInfo    [in] ������Ϣ
         *  @errorCode      [in] �������
         */
        virtual void onUnPulishFailed(AirJoyServiceInfo & serviceInfo, int errorCode);

};

#endif
