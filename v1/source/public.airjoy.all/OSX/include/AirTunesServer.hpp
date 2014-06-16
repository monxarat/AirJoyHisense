/*
* AirTunesServer.hpp
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

#ifndef __AIR_TUNES_SERVER_H__
#define __AIR_TUNES_SERVER_H__

#include <string>
#include "AirJoyApi.hpp"
#include "AirTunesServiceInfo.hpp"

namespace airjoy
{

    class AirTunesServerCore;
    class AirTunesServerListener;
    class AirTunesServiceInfo;
    class DacpClient;

    class AIRJOY_API AirTunesServer
    {
    public:
        typedef enum _DacpCommand
        {
            DACP_CMD_NEXTITEM   = 0x01,     // ��һ�׸���
            DACP_CMD_PREVITEM   = 0x02,     // ��һ�׸���
            DACP_CMD_PAUSE      = 0x03,     // ��ͣ
            DACP_CMD_PLAYPAUSE  = 0x04,     // ��ͣ����֮���л�
            DACP_CMD_PLAY       = 0x05,     // ����
            DACP_CMD_STOP       = 0x06,     // ֹͣ
            DACP_CMD_VOLUMEDOWN = 0x07,     // ����-
            DACP_CMD_VOLUMEUP   = 0x08,     // ����+
        } DacpCommand;

    public:
        AirTunesServer();
        virtual ~AirTunesServer();

        /**
         * ���������
         *  @serviceInfo    [in] AirTunes�������Ϣ
         *  @listener       [in] ������
         */
        void start(AirTunesServiceInfo &serviceInfo, AirTunesServerListener *listener);

        /**
         * ֹͣ�����
         */  
        void stop(void);

        /**
         * ��ȡ����˶˿�
         *  @return
         *      AirTunes����˶˿ں�
         */  
        uint16_t port(void);

        /**
         * ����DACP������Ϣ
         *  @ip             [in] iPhone/iPad IP��ַ
         *  @port           [in] iPhone/iPad �Ͻ�����Ƶ���������DACP�˿�
         */
        void setDacpInfo(const std::string &ip, uint16_t port);
        
        /**
         * ���DACP������Ϣ
         *  @ip             [in] iPhone/iPad IP��ַ
         */
        void cleanDacpInfo(const std::string &ip);

        /**
         * ������Ƶ��������
         *  @ip             [in] iPhone/iPad IP��ַ
         *  @cmd            [in] ��Ƶ��������
         */
        void sendDacpCommand(const std::string &ip, DacpCommand cmd);

    private:
        AirTunesServerCore      * m_core;
        DacpClient              * m_dacpClient;
        bool                      m_isStart;
    };

}

#endif // __AIR_TUNES_SERVER_H__