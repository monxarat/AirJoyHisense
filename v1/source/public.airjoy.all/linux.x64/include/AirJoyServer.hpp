/*
* AirJoyServer.hpp
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

#ifndef __AIRJOY_SERVER_H__
#define __AIRJOY_SERVER_H__

#include "AirJoyApi.hpp"
#include "AirJoyServiceInfo.hpp"
#include "AirFileListener.hpp"
#include "AirMediaListener.hpp"

namespace airjoy
{

    class AirJoyServerCore;

    class AIRJOY_API AirJoyServer
    {
    public:
        AirJoyServer();
        virtual ~AirJoyServer();
        
        /**
         * ���������
         *  @info           [in] AirJoy�������Ϣ
         */  
        bool start(AirJoyServiceInfo &info);
        
        /**
         * ֹͣ�����
         */ 
        bool stop(void);

        /**
         * ��ȡ����˶˿�
         *  @return
         *      AirJoy����˶˿ں�
         */
        uint16_t port(void);

        /**
         * ����ģ�飺AirFile
         *  @listener       [in] ������
         */ 
        bool loadAirFile(AirFileListener *listener);
        
        /**
         * ж��ģ�飺AirFile
         */ 
        bool unloadAirFile(void);

        /**
         * ����ģ�飺AirMedia
         *  @listener       [in] ������
         */ 
        bool loadAirMedia(AirMediaListener *listener);
        
        /**
         * ж��ģ�飺AirMedia
         */ 
        bool unloadAirMedia(void);

        /**
         * ����ý���¼�
         *  @mediaType  [in] �¼����ͣ�����Ϊ: [photo, audio, video, init]
         *  @action     [in] �¼�����, ����Ϊ��[play, pause, stop, init]
         *  @id         [in, option] ͼƬID����Ƶ��ID����Ƶ��ID
         *  @url        [in, option] ͼƬ��ַ����Ƶ��ַ����Ƶ��ַ
         *  @name       [in, option] ͼƬ���ơ���Ƶ���ơ���Ƶ����
         *  @fromIp     [in] ý����Դ��ַ
         */
        bool publishMediaEvent(const std::string &mediaType, 
                               const std::string &action, 
                               const std::string &id,
                               const std::string &url,
                               const std::string &name,
                               const std::string &fromIp);

    private:
        AirJoyServerCore    * m_core;
    };

}


#endif // __AIRJOY_SERVER_H__