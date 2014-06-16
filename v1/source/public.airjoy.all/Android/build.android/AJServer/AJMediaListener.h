
#ifndef __AJMEDIALISTENER_H__
#define __AJMEDIALISTENER_H__

#include <stdio.h>
#include <stdlib.h>

#include "AirMediaListener.hpp"


using namespace airjoy;

class AJMediaListener: public AirMediaListener
{
	    public:
	
        // �Ự��ӿ�

	    /**
         * �����¼�
         *  @fromIp         [in] ������IP
         *  @topic          [in] ��������
         */
        virtual void didSubscribeEvent(const std::string &fromIp, const std::string &topic);

	    /**
         * ȡ�������¼�
         *  @fromIp         [in] ������IP
         *  @topic          [in] ��������
         */        
        virtual void didUnsubscribeEvent(const std::string &fromIp, const std::string &topic);

        // ��Ƭ��ӿ�

        /**
         * ������Ƭ��
         *  @url            [in] ��Ƭ����ַ
         *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
         *  @name           [in] ��Ƭ����(��ѡ);
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode cachePhoto           (const std::string &photoId,
                                                 const std::string &url,
                                                 const std::string &name,const std::string &fromIp);

        /**
         * ��ʾ��Ƭ��
         *  @url            [in] ��Ƭ����ַ
         *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
         *  @name           [in] ��Ƭ����(��ѡ);
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode displayPhoto         (const std::string &photoId,
                                                 const std::string &url,
                                                 const std::string &name,const std::string &fromIp);
        /**
         * ��ʾ�Ѿ��������Ƭ��
         *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode displayCachedPhoto   (const std::string &photoId,const std::string &fromIp);

        /**
         * ֹͣ��ʾ��Ƭ
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode stopPhoto            (const std::string &fromIp);

        /**
         * ��ת��Ƭ
         *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
         *  @direction      [in] ��ת����
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode rotatePhoto          (const std::string &photoId,
                                                 PhotoDirectionType direction,const std::string &fromIp);

        /**
         * ������Ƭ
         *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
         *  @rate           [in] ���ű���
         *                       rate = 0: ����������ʴ�С
         *                       rate = 1: ������ԭ��С
         *                       rate > 1: �Ŵ�
         *                       0 < rate < 1, ��С
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode zoomPhoto            (const std::string &photoId,
                                                 float rate,const std::string &fromIp);



        virtual ResultCode movePhoto            (const std::string &photoId,
                                                 float x,
												 float y,const std::string &fromIp);

        // ��Ƶ��ӿ�

        /**
         * ������Ƶ��
         *  @url            [in] ��Ƶ����ַ
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @name           [in] ��Ƶ����(��ѡ);
         *  @position       [in] ����ʱ��㣬��λΪ��
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode playVideo            (const std::string &videoId,
                                                 const std::string &url,
                                                 const std::string &name,
                                                 float position,const std::string &fromIp);

        /**
         * ���ûط��ٶ�
         *  @url            [in] ��Ƶ����ַ
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @rate           [in] 0: ��ͣ��1: �������ʲ���
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode setPlayVideoRate     (const std::string &videoId,
                                                 float rate,const std::string &fromIp);

        /**
         * ֹͣ������Ƶ
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode stopPlayVideo        (const std::string &videoId,const std::string &fromIp);

        /**
         * ���ò��Ž���
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @playPosition   [in] ��ǰ������Ƶ��ʱ��㣬��λΪ��
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode setPlayVideoProgress  (const std::string &videoId,
                                                  float playPosition,const std::string &fromIp);

        /**
         * ��ȡ���Ž���
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @rate           [out] 0:��ͣ��1:����
         *  @position       [out] ��ǰ����/��ͣʱ��㣬��λΪ��
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode getPlayVideoProgress  (const std::string &videoId,
                                                  float *rate,
                                                  float *position,const std::string &fromIp);

        /**
         * ��ȡ������Ϣ
         *  @info           [out] ��Ƶ��Ϣ
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
//        virtual ResultCode getPlayVideoInfo       (AirJoyPlayBackInfo *info,const std::string &fromIp);

        virtual ResultCode getPlayVideoInfo       (AirJoyPlayBackInfo &info, const std::string &fromIp);
        // ��Ƶ��ӿ�

        /**
         * ������Ƶ��
         *  @url            [in] ��Ƶ����ַ
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @name           [in] ��Ƶ����(��ѡ);
         *  @position       [in] ����ʱ��㣬��λΪ��
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode playAudio            (const std::string &audioId,
                                                 const std::string &url,
                                                 const std::string &name,
                                                 float position,const std::string &fromIp);

        /**
         * ���ûط��ٶ�
         *  @url            [in] ��Ƶ����ַ
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @rate           [in] 0: ��ͣ��1: �������ʲ���
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode setPlayAudioRate     (const std::string &audioId,
                                                 float rate,const std::string &fromIp);

        /**
         * ֹͣ������Ƶ
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode stopPlayAudio        (const std::string &audioId,const std::string &fromIp);

        /**
         * ���ò��Ž���
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @playPosition   [in] ��ǰ������Ƶ��ʱ��㣬��λΪ��
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode setPlayAudioProgress  (const std::string &audioId,
                                                  float playPosition,const std::string &fromIp);

        /**
         * ��ȡ���Ž���
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @rate           [out] 0:��ͣ��1:����
         *  @position       [out] ��ǰ����/��ͣʱ��㣬��λΪ��
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode getPlayAudioProgress  (const std::string &audioId,
                                                  float *rate,
                                                  float *position,const std::string &fromIp);

        /**
         * ��ȡ������Ϣ
         *  @info           [out] ��Ƶ��Ϣ
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode getPlayAudioInfo       (AirJoyPlayBackInfo *info,const std::string &fromIp);

        // �����ӿ�

        /**
         * ��������
         *  @volume         [in] ��ǰ����(1-100);
         *  @return �ỰID
         */
        virtual ResultCode setVolume               (int volume,const std::string &fromIp);


        /**
         * ��ȡ����
         *  @volume         [out] ��ǰ����(1-100);
         *  @return �ỰID
         */
        virtual ResultCode getVolume               (int * volume,const std::string &fromIp);

};

#endif
