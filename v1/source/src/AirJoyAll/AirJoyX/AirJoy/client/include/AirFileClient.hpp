/*
* AirFileClient.hpp
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

#ifndef __AIRFILE_CLIENT_H__
#define __AIRFILE_CLIENT_H__

#include "AirJoyClient.hpp"

namespace airjoy
{


    class AirFileClientListener;

    class AIRJOY_API AirFileClient : public AirJoyClient
    {
    public:
        AirFileClient();
        virtual ~AirFileClient();
        
        /**
         * 设置监听者
         *  @listener     [in] 监听者
         */
        void setListener(AirFileClientListener *listener);

		/**
         * 连接服务端
         *  @ip             [in] 服务端地址
         *  @port           [in] 服务端口
         *  @return
         *          - true 函数执行成功
         *          - false 函数执行失败
         */
        bool connect(const std::string &ip, uint16_t port);

        /**
         * 断开连接
         *  @return
         *          - true 函数执行成功
         *          - false 函数执行失败
         */
        bool disconnect(void);

        /**
         * 发送消息
         */
        bool sendMessage(const std::string &message);
    };


}

#endif // __AIRFILE_CLIENT_H__
