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
         * ���ü�����
         *  @listener     [in] ������
         */
        void setListener(DacpClientListener *listener);

        /**
         * ���DACP������IP�Ͷ˿�
         *  @ip         [in]ip
         *  @port       [in]�˿�
         */
        void addServer(const std::string &ip, uint16_t port);

        /**
         * ɾ��DACP������
         *  @ip         [in]�����ip
         */
        void removeServer(const std::string &ip);

        /**
         * �����ͻ���
         */    
        void start(void);
        
        /**
         * ֹͣ�ͻ���
         */    
        void stop(void);

        /**
         * ������Ϣ
         *  @message    [in]��Ϣ���� 
         *  @return
         *      - true    ���ͳɹ�
         *      - false   ����ʧ��
         *  @note ����Ѿ����ӵ���IP�������ֱ�ӷ��ͣ�����������
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

