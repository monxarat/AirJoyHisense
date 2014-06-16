/*
* AirMediaListener.hpp
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

#ifndef __AIR_MEDIA_LISTENER_H__
#define __AIR_MEDIA_LISTENER_H__

#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"

namespace airjoy
{

    class AIRJOY_API AirMediaListener
    {
    public:
        AirMediaListener() {}
        virtual ~AirMediaListener() {}

    public:
	
		//------------------------------------------------------------------------------------
        // ������ӿ�
        //------------------------------------------------------------------------------------	

	    /**
         * �����¼�
         *  @fromIp         [in] ������IP
         *  @topic          [in] ��������
         */
        virtual void didSubscribeEvent(const std::string &fromIp, const std::string &topic)
        {
        }

	    /**
         * ȡ�������¼�
         *  @fromIp         [in] ������IP
         *  @topic          [in] ��������
         */        
        virtual void didUnsubscribeEvent(const std::string &fromIp, const std::string &topic)
        {
        }

        //------------------------------------------------------------------------------------
        // ��Ƭ��ӿ�
        //------------------------------------------------------------------------------------

        /**
         * ������Ƭ��
         *  @url            [in] ��Ƭ����ַ
         *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
         *  @name           [in] ��Ƭ����(��ѡ)
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode cachePhoto           (const std::string &photoId,
                                                 const std::string &url,
                                                 const std::string &name,
                                                 const std::string &fromIp)
        { 
            return ResultCodeErrorNotSupport; 
        }

        /**
         * ��ʾ��Ƭ��
         *  @url            [in] ��Ƭ����ַ
         *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
         *  @name           [in] ��Ƭ����(��ѡ)
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode displayPhoto         (const std::string &photoId,
                                                 const std::string &url,
                                                 const std::string &name,
                                                 const std::string &fromIp)
        { 
            return ResultCodeErrorNotSupport; 
        }

        /**
         * ��ʾ�Ѿ��������Ƭ��
         *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode displayCachedPhoto   (const std::string &photoId,
                                                 const std::string &fromIp)
        { 
            return ResultCodeErrorNotSupport; 
        }

        /**
         * ֹͣ��ʾ��Ƭ
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode stopPhoto            (const std::string &fromIp)
        { 
            return ResultCodeErrorNotSupport; 
        }

        /**
         * ��ת��Ƭ
         *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
         *  @direction      [in] ��ת����
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode rotatePhoto          (const std::string &photoId,
                                                 PhotoDirectionType direction,
                                                 const std::string &fromIp)
        { 
            return ResultCodeErrorNotSupport; 
        }

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
                                                 float rate,
                                                 const std::string &fromIp)
        {
            return ResultCodeErrorNotSupport; 
        }

        /**
         * �ƶ���Ƭ
         *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
         *  @x              [in] x���ƶ��������з��Ÿ���������λ�����շ���Ļ���
��       *                       xֵ��Χ��(-1 < x < 1) 
��       *                       x = 0: ���ƶ�
��       *                       0 < x < 1: �����ƶ�
��       *                       ��-1 < x < 0: �����ƶ�
��       *                       ������շ���Ļ���Ϊ800����x = 0.3������ͼƬ����Ļ�ҷ��ƶ���0.3 x 800�������ص㡣
��       *  @y             [ in] y���ƶ��������з��Ÿ���������λ�����շ���Ļ�߶�
��       *                     ��yֵ��Χ��(-1 < y < 1) 
��       *                     ��y = 0: ���ƶ�
��       *                       0 < y < 1: �����ƶ�
��       *                     ��-1 < y < 0: �����ƶ�
��       *                     ��������շ���Ļ���Ϊ600����y = 0.3������ͼƬ����Ļ�·��ƶ���0.3 x 600�������ص㡣
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode movePhoto            (const std::string &photoId,
                                                 float x,
												 float y,
                                                 const std::string &fromIp)
        {
            return ResultCodeErrorNotSupport; 
        }

        //------------------------------------------------------------------------------------
        // ��Ƶ��ӿ�
        //------------------------------------------------------------------------------------

        /**
         * ������Ƶ��
         *  @url            [in] ��Ƶ����ַ
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @name           [in] ��Ƶ����(��ѡ)
         *  @position       [in] ����ʱ��㣬��λΪ��
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode playVideo            (const std::string &videoId,
                                                 const std::string &url,
                                                 const std::string &name,
                                                 float position,
                                                 const std::string &fromIp)
        { 
            return ResultCodeErrorNotSupport; 
        }

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
                                                 float rate,
                                                 const std::string &fromIp)
        {
            return ResultCodeErrorNotSupport;
        }

        /**
         * ֹͣ������Ƶ
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode stopPlayVideo        (const std::string &videoId,
                                                 const std::string &fromIp)
        {
            return ResultCodeErrorNotSupport;
        }

        /**
         * ���ò��Ž���
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @playPosition   [in] ��ǰ������Ƶ��ʱ��㣬��λΪ��
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode setPlayVideoProgress  (const std::string &videoId,
                                                  float playPosition,
                                                  const std::string &fromIp)
        {
            return ResultCodeErrorNotSupport;
        }

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
                                                  float *position,
                                                  const std::string &fromIp)
        {
            return ResultCodeErrorNotSupport;
        }

        /**
         * ��ȡ������Ϣ
         *  @info           [out] ��Ƶ��Ϣ
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode getPlayVideoInfo       (AirJoyPlayBackInfo &info,
                                                   const std::string &fromIp)
        {
            return ResultCodeErrorNotSupport;
        }

        //------------------------------------------------------------------------------------
        // ��Ƶ��ӿ�
        //------------------------------------------------------------------------------------

        /**
         * ������Ƶ��
         *  @url            [in] ��Ƶ����ַ
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @name           [in] ��Ƶ����(��ѡ)
         *  @position       [in] ����ʱ��㣬��λΪ��
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode playAudio            (const std::string &audioId,
                                                 const std::string &url,
                                                 const std::string &name,
                                                 float position,
                                                 const std::string &fromIp)
        { 
            return ResultCodeErrorNotSupport; 
        }

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
                                                 float rate,
                                                 const std::string &fromIp)
        { 
            return ResultCodeErrorNotSupport; 
        }

        /**
         * ֹͣ������Ƶ
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode stopPlayAudio        (const std::string &audioId,
                                                 const std::string &fromIp)
        {
            return ResultCodeErrorNotSupport;
        }

        /**
         * ���ò��Ž���
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @playPosition   [in] ��ǰ������Ƶ��ʱ��㣬��λΪ��
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode setPlayAudioProgress  (const std::string &audioId,
                                                  float playPosition,
                                                  const std::string &fromIp)
        {
            return ResultCodeErrorNotSupport;
        }

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
                                                  float *position,
                                                  const std::string &fromIp)
        {
            return ResultCodeErrorNotSupport;
        }

        /**
         * ��ȡ������Ϣ
         *  @info           [out] ��Ƶ��Ϣ
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode getPlayAudioInfo       (AirJoyPlayBackInfo &info,
                                                   const std::string &fromIp)
        {
            return ResultCodeErrorNotSupport;
        }

        //------------------------------------------------------------------------------------
        // �����ӿ�
        //------------------------------------------------------------------------------------

        /**
         * ��������
         *  @volume         [in] ��ǰ����(1-100)
         *  @return �������
         */
        virtual ResultCode setVolume               (int volume,
                                                    const std::string &fromIp)
        {
            return ResultCodeErrorNotSupport;
        }


        /**
         * ��ȡ����
         *  @volume         [out] ��ǰ����(1-100)
         *  @return �������
         */
        virtual ResultCode getVolume               (int * volume,
                                                    const std::string &fromIp)
        {
            return ResultCodeErrorNotSupport;
        }

        //------------------------------------------------------------------------------------
        // �Զ�����Ϣ
        //------------------------------------------------------------------------------------

        /**
         * �յ�һ���Զ�������
         *  @appAction		[in] �Զ�����Ϊ
		 *  @appParam		[in] �Զ������
		 *  @appResult		[out] ���
	     *  @appResultLen	[in] appResult����
         *  @return
		 *		true  - ֧�ִ��Զ�������
		 *		false - ��֧�ִ��Զ�������
         */
        virtual bool didReceiveRequest			   (const std::string &appAction,
			                                        const std::string &appParam,
													char *appResult,
                                                    size_t appResultLen,
                                                    const std::string &fromIp)
        {
            return false;
        }
    };

}

#endif // __AIR_MEDIA_LISTENER_H__

