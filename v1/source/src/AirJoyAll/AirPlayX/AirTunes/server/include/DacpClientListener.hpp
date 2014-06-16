/* 
* DacpClientListener.h
*
* coding.tom@gmail.com
* 2011-8-30
*
*/

#ifndef __DACP_CLIENT_LISTENER_H__
#define __DACP_CLIENT_LISTENER_H__

#include "AirJoyTypeDef.hpp"

namespace airjoy
{

    class DacpClientListener
    {
    public:
        DacpClientListener() {}
        virtual ~DacpClientListener() {}

        /**
         * ���ӳɹ�
         */
		virtual void didConnectOk(void) = 0;

        /**
         * �Ͽ�����
         */
		virtual void didDisConnect(void) = 0;

        /**
         * ��ʱ
         */
		virtual void didTimeout(void) = 0;
		
		/**
         * �������
         */
		virtual void didErrorNet(void) = 0;

        /**
         * ���Ӵ���
         */
        virtual void didConnectFailed(void) = 0;

    };

}

#endif // __DACP_CLIENT_LISTENER_H__

