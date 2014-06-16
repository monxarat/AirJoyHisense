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
         * ��Ϣ����
         */
        typedef enum _MessageType
        {
            MessageGet     = 0,     // ��ȡ
            MessageSet     = 1,     // ����
        } MessageType;

    public:
        AirJoyClient();
        virtual ~AirJoyClient();

        /**
         * �����ͻ���
         */    
        void start(void);
        
        /**
         * ֹͣ�ͻ���
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
