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
         * ���ü�����
         *  @listener     [in] ������
         */
        void setListener(AirFileClientListener *listener);

		/**
         * ���ӷ����
         *  @ip             [in] ����˵�ַ
         *  @port           [in] ����˿�
         *  @return
         *          - true ����ִ�гɹ�
         *          - false ����ִ��ʧ��
         */
        bool connect(const std::string &ip, uint16_t port);

        /**
         * �Ͽ�����
         *  @return
         *          - true ����ִ�гɹ�
         *          - false ����ִ��ʧ��
         */
        bool disconnect(void);

        /**
         * ������Ϣ
         */
        bool sendMessage(const std::string &message);
    };


}

#endif // __AIRFILE_CLIENT_H__
