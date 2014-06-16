/*
* DacpClient.hpp
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

#ifndef __DACP_CLIENT_H__
#define __DACP_CLIENT_H__

#include <string>
#include <map>

#include "AirJoyTypeDef.hpp"

namespace airjoy
{

    class DacpClientCore;
    class DacpClientListener;

    class DacpClient
    {
    public:
        DacpClient();
        virtual ~DacpClient();

        /**
         * 设置监听者
         *  @listener     [in] 监听者
         */
        void setListener(DacpClientListener *listener);

        /**
         * 添加DACP服务器IP和端口
         *  @ip         [in]ip
         *  @port       [in]端口
         */
        void addServer(const std::string &ip, uint16_t port);

        /**
         * 删除DACP服务器
         *  @ip         [in]服务端ip
         */
        void removeServer(const std::string &ip);

        /**
         * 启动客户端
         */    
        void start(void);
        
        /**
         * 停止客户端
         */    
        void stop(void);

        /**
         * 发送消息
         *  @message    [in]消息内容 
         *  @return
         *      - true    发送成功
         *      - false   发送失败
         *  @note 如果已经连接到此IP，则这次直接发送，不再重连。
         */
		bool send(const std::string &ip, const std::string &message);

    private:
        DacpClientCore       * m_core;
        bool                   m_isStart;

        typedef std::map<std::string, uint16_t> DacpServerMap;
        DacpServerMap          m_dacpServer;
    };
}

#endif // __DACP_CLIENT_H__

