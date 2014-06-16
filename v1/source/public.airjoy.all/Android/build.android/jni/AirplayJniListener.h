
#ifndef __AIRPLAYJNILISTENER_H__
#define __AIRPLAYJNILISTENER_H__

#include "AirPlayServer.hpp"
#include "AirPlayServerListener.hpp"
#include "AirPlayNsd.hpp"


using namespace airjoy;

class AirplayJniListener:public AirPlayServerListener 
{

    public:

        /**
         * �����¼�
         *  @clientIp��        �ͻ���ip
         */
        virtual void didSubscribeEvent(const std::string &clientIp);

        /**
         * ȡ�������¼�
         *  @clientIp��        �ͻ���ip
         */
        virtual void didUnsubscribeEvent(const std::string &clientIp);

        /**
         * ����������Ƭ
         *  @photoId��         ��Ƭid
         */
        virtual void willPutPhoto(const std::string &photoId, const std::string &clientIp) ;

        /*
         * ������Ƭ����Ҫ������ʾ��
         *  @photoId:            ��Ƭid
         *  @sessionId:          �Ựid
         *  @data:               ��Ƭԭʼ����(jpg��ʽ��
         *  @length:             ���ݴ�С
         *  @note:
         *       ������շ������ر���Ƭ������Ҫ�����¼�: AirPlayServer::publishEvent();
         */
        virtual void didPutPhoto(const std::string &photoId, const char *data, std::size_t length, const std::string &clientIp) ;
        
        /**
         * ����������Ƭ
         *  @photoId��         ��Ƭid
         */
        virtual void willPutPhotoCacheOnly(const std::string &photoId, const std::string &clientIp) ;

        /**
         * ������Ƭ�����棨��������ʾ������������
         *  @photoId:            ��Ƭid
         *  @sessionId:          �Ựid
         *  @data:               ��Ƭԭʼ����(jpg��ʽ��
         *  @length:             ���ݴ�С
         */
        virtual void didPutPhotoCacheOnly(const std::string &photoId,  const char *data, std::size_t length, const std::string &clientIp) ;

        /**
         * ��ʾ�Ѿ��������Ƭ
         *  @photoId:            ��Ƭid
         */
        virtual void didDisplayCachedPhoto(const std::string &photoId,  const std::string &clientIp) ;
        
        /**
         * ֹͣ��ʾͼƬ��ֹͣ�õ�Ƭ��ʾ
         */
       // virtual void didStopPhotoOrSlideshow(void)  ;

        /**
         * ��ʼ������Ƶ
         *  @startPosition:   ��startPostion��ָ��ʱ��㿪ʼ����
         *  @contentLocation: ��Ƶλ�ã��ٷֱ�: ��ǰλ��/�ܳ��ȣ�
         *     1: http://192.168.0.116:7001/1/1a59ea26-1861-5206-aa61-ce3960ae6cb1.mp4
         *     2: http://v.youku.com/player/getRealM3U8/vid/XNDA3MjEyMTMy/type/flv/ts/1340956945/video.m3u8
         *  @sessionId��      �ỰID
         */
        virtual void didStartPlayVideo(const std::string &contentLocation, double startPosition,  const std::string &clientIp) ;

        /**
         * ��ʼ������Ƶ
         *  @contentLocation    [in] ��Ƶλ�ã��磺http://10.0.1.2:7002/123.mp3
         *  @startPosition      [in] ��startPostion��ָ��ʱ��㿪ʼ���ţ���λΪ�ٷֱ�: ��ǰλ��/�ܳ��ȣ�
         *  @clientIp           [in] �ͻ���ip
         */
        virtual void didStartPlayMusic(const std::string &contentLocation, double startPosition, const std::string &clientIp) ;


        /**
         * ������Ƶ�ط��ٶ�
         *  @rate: 
         *     0 - ��ͣ
         *     1 - �������ʲ���
         */
        virtual void didSetPlaybackRate(float rate, const std::string &clientIp) ;

        /**
         * ֹͣ�ط�
         */
        //virtual void didStopPlayback(void, const std::string &clientIp) ;

        /**
         * ֹͣ�طŻ�ֹͣ��ʾ��Ƭ
         *  @clientIp           [in] �ͻ���ip
         */
        virtual void didStop(const std::string &clientIp);

        
        /**
         * ���õ�ǰ���Ž���
         *  @playPostion        ��ǰ���Ž��ȣ���λ���룩
         */
        virtual void setCurrentPlaybackProgress(float playPosition, const std::string &clientIp) ;

        /**
         * ��ȡ��ǰ���Ž��ȣ���������ܶ�ȡ��Ƶ���ȣ�����0����
         *  @time       [out] position : ��ǰ���ŵ㣨��λ���룩
         *              [out] duration : ��Ƶ�ܳ��ȣ���λ���룩
         */
        virtual void getCurrentPlaybackProgress(PlaybackTime &time, const std::string &clientIp) ;

        /**
         * ��ȡ�ط���Ϣ
         *  @info       [out] �ط���Ϣ
         */
        virtual void getPlaybackInfo(PlaybackInfo &info, const std::string &clientIp) ;

        /**
         * �����������ٷֱ�: ��ǰ����/��������
         *  @value      [in] �ٷֱ�: ��ǰ����/��������
         */
        virtual void didSetVolume(float value, const std::string &clientIp) ;





        /**
         * ��ʼ�õ�Ƭ����
         *  @slideDuration      [in] ÿ��ͼƬͣ����ʱ��
         *  @clientIp           [in] �ͻ���ip
         */
        virtual void didStartSlideshows(int slideDuration, const std::string &clientIp);
        
        /**
         * ֹͣ�õ�Ƭ����
         *  @clientIp           [in] �ͻ���ip
         */
        virtual void didStopSlideshows(const std::string &clientIp);
        
        /**
         * ��ȡ��һ�Żõ�Ƭ���ŵ�ͼƬ
         *  @index              [in] ��Ƭ��������1�ţ���2�ţ���3��...
         *  @data               [in] ��Ƭԭʼ����(jpg��ʽ��
         *  @length             [in] ���ݴ�С
         *  @clientIp           [in] �ͻ���ip
         *  @note
         *      1. ����AirPlayServer::getSlideshowsPicture(clientIp)�����յ�����Ϣ
         */
        virtual void didGetSlideshowsPicture(int index, const char *data, std::size_t length, const std::string &clientIp);
        
        /**
         * ��ȡ�õ�ƬͼƬʧ��
         *  @clientIp           [in] �ͻ���ip
         *  @note
         *      1. ����AirPlayServer::getSlideshowsPicture(clientIp)�����յ�����Ϣ
         *      2. �յ������Ϣ����ʾ�õ�Ƭ����Ƭ�Ѿ��������
         */
        virtual void didGetSlideshowsPictureFailed(const std::string &clientIp);

};


#endif
