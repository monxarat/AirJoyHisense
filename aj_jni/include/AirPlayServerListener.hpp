/*
* AirPlayServerListener.hpp
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
*
*/

#ifndef __AIRPLAY_SERVER_LISTENER_H__
#define __AIRPLAY_SERVER_LISTENER_H__

#include <string>
#include "AirJoyApi.hpp"

namespace airjoy
{

    // ʱ�䷶Χ
    typedef struct AIRJOY_API _TimeRanges
    {
        float startPosition;    // ��ʼʱ��㣬��λ: seconds
        float duration;         // ����ʱ�䳤�ȣ���λ: seconds
    } TimeRanges;
    
    // ����ʱ���
    typedef struct AIRJOY_API _PlaybackTime
    {
        float playPosition;     // ���ڲ��ŵ�ʱ��㣬��λ: seconds
        float duration;         // ��Ƶ�ܳ��ȣ���λ: seconds
    } PlaybackTime;

    // �ط���Ϣ
    typedef struct AIRJOY_API _PlaybackInfo
    {
        PlaybackTime playbackTime;      // ��Ƶ�ܳ��� & ���ڲ��ŵ�ʱ���
        float rate;                     // 0 - ��ͣ, 1 - �������ʲ���
        bool readyToPlay;	            // true - �Ѿ�׼������, false - ��δ׼�����ţ���ȱʡֵ��true��
        bool playbackBufferEmpty;       // true - ���Ż���գ�false - ���Ż���ǿգ���ȱʡֵ��true��
        bool playbackBufferFull;        // true - ���Ż�������false - ���Ż���δ������ȱʡֵ��false��
        bool playbackLikelyToKeepUp;    // true��ȱʡֵ��true��
        TimeRanges loadedTimeRanges;    // ��Ƶ���س��� & ��Ƶ��ʼ���ŵ�ʱ���
        TimeRanges seekableTimeRanges;  // iPhone/iPad�˿��Ի�����Ƶ��ʱ�䷶Χ����Ƶ���� & ��ʼ�㣩
    } PlaybackInfo;

    class AIRJOY_API AirPlayServerListener
    {
    public:
        AirPlayServerListener() {}
        virtual ~AirPlayServerListener() {}

    public:

        /**
         * �����¼�
         *  @clientIp           [in] �ͻ���ip
         */
        virtual void didSubscribeEvent(const std::string &clientIp) {}

        /**
         * ȡ�������¼�
         *  @clientIp           [in] �ͻ���ip
         */
        virtual void didUnsubscribeEvent(const std::string &clientIp) {}

        /**
         * ����������Ƭ
         *  @photoId            [in] ��Ƭid
         *  @clientIp           [in] �ͻ���ip
         */
        virtual void willPutPhoto(const std::string &photoId, const std::string &clientIp) = 0;

        /*
         * ������Ƭ����Ҫ������ʾ��
         *  @photoId            [in] ��Ƭid
         *  @data               [in] ��Ƭԭʼ����(jpg��ʽ��
         *  @length             [in] ���ݴ�С
         *  @clientIp           [in] �ͻ���ip
         *  @note
         *      ������շ��������¼���
         *        1��ֹͣ��ʾ��Ƭ
         *        2��ֹͣ������Ƶ
         *        3����ͣ��Ƶ
         *        4��������Ƶ
         *      ����Ҫ�����¼�: AirPlayServer::publishEvent();
         */
        virtual void didPutPhoto(const std::string &photoId, const char *data, std::size_t length, const std::string &clientIp) = 0;
        
        /**
         * ����������Ƭ
         *  @photoId            [in] ��Ƭid
         *  @clientIp           [in] �ͻ���ip
         */
        virtual void willPutPhotoCacheOnly(const std::string &photoId, const std::string &clientIp) = 0;

        /**
         * ������Ƭ�����棨��������ʾ������������
         *  @photoId            [in] ��Ƭid
         *  @data               [in] ��Ƭԭʼ����(jpg��ʽ��
         *  @length             [in] ���ݴ�С
         *  @clientIp           [in] �ͻ���ip
         */
        virtual void didPutPhotoCacheOnly(const std::string &photoId, const char *data, std::size_t length, const std::string &clientIp) = 0;

        /**
         * ��ʾ�Ѿ��������Ƭ
         *  @photoId            [in] ��Ƭid
         *  @clientIp           [in] �ͻ���ip
         */
        virtual void didDisplayCachedPhoto(const std::string &photoId, const std::string &clientIp) = 0;
        
        /**
         * ��ʼ������Ƶ
         *  @contentLocation    [in] ��Ƶλ�ã���λΪ�ٷֱ�: ��ǰλ��/�ܳ��ȣ�
         *  @startPosition      [in] ��startPostion��ָ��ʱ��㿪ʼ����
         *  @clientIp           [in] �ͻ���ip
         */
        virtual void didStartPlayVideo(const std::string &contentLocation, double startPosition, const std::string &clientIp) = 0;
        
        /**
         * ��ʼ������Ƶ
         *  @contentLocation    [in] ��Ƶλ�ã��磺http://10.0.1.2:7002/123.mp3
         *  @startPosition      [in] ��startPostion��ָ��ʱ��㿪ʼ���ţ���λΪ�ٷֱ�: ��ǰλ��/�ܳ��ȣ�
         *  @clientIp           [in] �ͻ���ip
         */
        virtual void didStartPlayMusic(const std::string &contentLocation, double startPosition, const std::string &clientIp) = 0;

        /**
         * ���ûط��ٶ�
         *  @rate: 
         *     0 - ��ͣ
         *     1 - �������ʲ���
         *  @clientIp           [in] �ͻ���ip
         */
        virtual void didSetPlaybackRate(float rate, const std::string &clientIp) = 0;

        /**
         * ֹͣ�طŻ�ֹͣ��ʾ��Ƭ
         *  @clientIp           [in] �ͻ���ip
         */
        virtual void didStop(const std::string &clientIp) = 0;
        
        /**
         * ���õ�ǰ���Ž���
         *  @playPostion        [in] ��ǰ���Ž��ȣ���λ���룩
         *  @clientIp           [in] �ͻ���ip
         */
        virtual void setCurrentPlaybackProgress(float playPosition, const std::string &clientIp) = 0;

        /**
         * ��ȡ��ǰ���Ž��ȣ���������ܶ�ȡ��Ƶ���ȣ�����0����
         *  @time               [out] position : ��ǰ���ŵ㣨��λ���룩
         *                      [out] duration : ��Ƶ�ܳ��ȣ���λ���룩
         *  @clientIp           [in] �ͻ���ip
         */
        virtual void getCurrentPlaybackProgress(PlaybackTime &time, const std::string &clientIp) = 0;

        /**
         * ��ȡ�ط���Ϣ
         *  @info               [out] �ط���Ϣ
         *  @clientIp           [in] �ͻ���ip
         */
        virtual void getPlaybackInfo(PlaybackInfo &info, const std::string &clientIp) = 0;

        /**
         * �����������ٷֱ�: ��ǰ����/��������
         *  @value              [in] �ٷֱ�: ��ǰ����/��������
         *  @clientIp           [in] �ͻ���ip
         */
        virtual void didSetVolume(float value, const std::string &clientIp) = 0;

        /**
         * ��ʼ�õ�Ƭ����
         *  @slideDuration      [in] ÿ��ͼƬͣ����ʱ��
         *  @clientIp           [in] �ͻ���ip
         */
        virtual void didStartSlideshows(int slideDuration, const std::string &clientIp) = 0;
        
        /**
         * ֹͣ�õ�Ƭ����
         *  @clientIp           [in] �ͻ���ip
         */
        virtual void didStopSlideshows(const std::string &clientIp) = 0;
        
        /**
         * ��ȡ��һ�Żõ�Ƭ���ŵ�ͼƬ
         *  @index              [in] ��Ƭ��������1�ţ���2�ţ���3��...
         *  @data               [in] ��Ƭԭʼ����(jpg��ʽ��
         *  @length             [in] ���ݴ�С
         *  @clientIp           [in] �ͻ���ip
         *  @note
         *      1. ����AirPlayServer::getSlideshowsPicture(clientIp)�����յ�����Ϣ
         */
        virtual void didGetSlideshowsPicture(int index, const char *data, std::size_t length, const std::string &clientIp) = 0;
        
        /**
         * ��ȡ�õ�ƬͼƬʧ��
         *  @clientIp           [in] �ͻ���ip
         *  @note
         *      1. ����AirPlayServer::getSlideshowsPicture(clientIp)�����յ�����Ϣ
         *      2. �յ������Ϣ����ʾ�õ�Ƭ����Ƭ�Ѿ��������
         */
        virtual void didGetSlideshowsPictureFailed(const std::string &clientIp) = 0;

    };

}

#endif // __AIRPLAY_SERVER_LISTENER_H__