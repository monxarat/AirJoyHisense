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
         * 连接成功
         */
		virtual void didConnectOk(void) = 0;

        /**
         * 断开连接
         */
		virtual void didDisConnect(void) = 0;

        /**
         * 超时
         */
		virtual void didTimeout(void) = 0;
		
		/**
         * 网络错误
         */
		virtual void didErrorNet(void) = 0;

        /**
         * 连接错误
         */
        virtual void didConnectFailed(void) = 0;

    };

}

#endif // __DACP_CLIENT_LISTENER_H__

