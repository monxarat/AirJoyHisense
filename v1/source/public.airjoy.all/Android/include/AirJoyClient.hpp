/*
* AirJoyClient.hpp
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

#ifndef __AIRJOY_CLIENT_H__
#define __AIRJOY_CLIENT_H__

#include <string>
#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"


namespace airjoy
{

    class AirJoyClientCore;

    class AIRJOY_API AirJoyClient
    {
    public:

        /**
         * 消息类型
         */
        typedef enum _MessageType
        {
            MessageGet     = 0,     // 读取
            MessageSet     = 1,     // 发送
        } MessageType;

    public:
        AirJoyClient();
        virtual ~AirJoyClient();

        /**
         * 启动客户端
         */    
        void start(void);
        
        /**
         * 停止客户端
         */    
        void stop(void);

        /**
         * AirJoyClientCore
         */
        AirJoyClientCore * core(void);

    private:
        AirJoyClientCore       * m_core;
        //AirJoySessionId          m_sessionId;
        bool                     m_isStart;
    };
}

#endif // __AIRJOY_CLIENT_H__
