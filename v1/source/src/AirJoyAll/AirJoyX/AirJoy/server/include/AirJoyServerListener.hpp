/* 
* AirJoyServerListener.h
*
* coding.tom@gmail.com
* 2011-8-30
*
*/

#ifndef __AIRJOY_SERVER_LISTENER_H__
#define __AIRJOY_SERVER_LISTENER_H__

#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"

namespace airjoy
{

    class AIRJOY_API AirJoyServerListener
    {
    public:
        // �ط���Ϣ
        typedef struct _PlayInfo
        {
            std::string     id;         // ��Ƶ/��ƵID
            std::string     url;        // ��ַ
            std::string     name;       // ���ƣ���ѡ��
            float           rate;       // 0:��ͣ, 1:����
            float           position;   // ��ǰ����/��ͣʱ��㣬��λΪ�롣
        } PlayInfo;

    public:
        AirJoyServerListener() {}
        virtual ~AirJoyServerListener() {}

    public:
        //------------------------------------------------------------------------------------
        // ���֧��ĳ�๦�ܣ���Ҫ���أ�true
        //------------------------------------------------------------------------------------
        virtual bool isSupportPhoto             (void)  { return false; }
        virtual bool isSupportVideo             (void)  { return false; }
        virtual bool isSupportAudio             (void)  { return false; }
        virtual bool isSupportVolume            (void)  { return false; }

        //------------------------------------------------------------------------------------
        // ��Ƭ��ӿ�
        //------------------------------------------------------------------------------------

        /**
         * ������Ƭ��
         *  @fromDeviceId   [in] ���ͷ����豸ID
         *  @url            [in] ��Ƭ����ַ
         *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
         *  @name           [in] ��Ƭ����(��ѡ)
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode cachePhoto           (const std::string &fromDeviceId,
                                                 const std::string &photoId,
                                                 const std::string &url,
                                                 const std::string &name)
        { 
            return ResultCodeErrorNotSupport; 
        }

        /**
         * ��ʾ��Ƭ��
         *  @fromDeviceId   [in] ���ͷ����豸ID
         *  @url            [in] ��Ƭ����ַ
         *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
         *  @name           [in] ��Ƭ����(��ѡ)
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode displayPhoto         (const std::string &fromDeviceId,
                                                 const std::string &photoId,
                                                 const std::string &url,
                                                 const std::string &name)
        { 
            return ResultCodeErrorNotSupport; 
        }

        /**
         * ��ʾ�Ѿ��������Ƭ��
         *  @fromDeviceId   [in] ���ͷ����豸ID
         *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode displayCachedPhoto   (const std::string &fromDeviceId,
                                                 const std::string &photoId)
        { 
            return ResultCodeErrorNotSupport; 
        }

        /**
         * ֹͣ��ʾ��Ƭ
         *  @fromDeviceId   [in] ���ͷ����豸ID
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode stopPhoto            (const std::string &fromDeviceId)
        { 
            return ResultCodeErrorNotSupport; 
        }

        /**
         * ��ת��Ƭ
         *  @fromDeviceId   [in] ���ͷ����豸ID
         *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
         *  @direction      [in] ��ת����
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode rotatePhoto          (const std::string &fromDeviceId,
                                                 const std::string &photoId,
                                                 PhotoDirectionType direction)
        { 
            return ResultCodeErrorNotSupport; 
        }

        /**
         * ��ת��Ƭ
         *  @fromDeviceId   [in] ���ͷ����豸ID
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
        virtual ResultCode zoomPhoto            (const std::string &fromDeviceId,
                                                 const std::string &photoId,
                                                 float rate)
        {
            return ResultCodeErrorNotSupport; 
        }

        //------------------------------------------------------------------------------------
        // ��Ƶ��ӿ�
        //------------------------------------------------------------------------------------

        /**
         * ������Ƶ��
         *  @fromDeviceId   [in] ���ͷ����豸ID
         *  @url            [in] ��Ƶ����ַ
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @name           [in] ��Ƶ����(��ѡ)
         *  @position       [in] ����ʱ��㣬��λΪ��
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode playVideo            (const std::string &fromDeviceId,
                                                 const std::string &videoId,
                                                 const std::string &url,
                                                 const std::string &name,
                                                 float position)
        { 
            return ResultCodeErrorNotSupport; 
        }

        /**
         * ���ûط��ٶ�
         *  @fromDeviceId   [in] ���ͷ����豸ID
         *  @url            [in] ��Ƶ����ַ
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @rate           [in] 0: ��ͣ��1: �������ʲ���
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode setPlayVideoRate     (const std::string &fromDeviceId,
                                                 const std::string &videoId,
                                                 float rate)
        {
            return ResultCodeErrorNotSupport;
        }

        /**
         * ֹͣ������Ƶ
         *  @toDeviceId     [in] ���շ����豸ID
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode stopPlayVideo        (const std::string &fromDeviceId,
                                                 const std::string &videoId)
        {
            return ResultCodeErrorNotSupport;
        }

        /**
         * ���ò��Ž���
         *  @toDeviceId     [in] ���շ����豸ID
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @playPosition   [in] ��ǰ������Ƶ��ʱ��㣬��λΪ��
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode setPlayVideoProgress  (const std::string &fromDeviceId,
                                                  const std::string &videoId,
                                                  float playPosition)
        {
            return ResultCodeErrorNotSupport;
        }

        /**
         * ��ȡ���Ž���
         *  @toDeviceId     [in] ���շ����豸ID
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @rate           [out] 0:��ͣ��1:����
         *  @position       [out] ��ǰ����/��ͣʱ��㣬��λΪ��
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode getPlayVideoProgress  (const std::string &fromDeviceId,
                                                  const std::string &videoId,
                                                  float *rate,
                                                  float *position)
        {
            return ResultCodeErrorNotSupport;
        }

        /**
         * ��ȡ������Ϣ
         *  @toDeviceId     [in] ���շ����豸ID
         *  @info           [out] ��Ƶ��Ϣ
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode getPlayVideoInfo       (const std::string &toDeviceId,
                                                   PlayInfo *info)
        {
            return ResultCodeErrorNotSupport;
        }

        //------------------------------------------------------------------------------------
        // ��Ƶ��ӿ�
        //------------------------------------------------------------------------------------

        /**
         * ������Ƶ��
         *  @fromDeviceId   [in] ���ͷ����豸ID
         *  @url            [in] ��Ƶ����ַ
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @name           [in] ��Ƶ����(��ѡ)
         *  @position       [in] ����ʱ��㣬��λΪ��
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode playAudio            (const std::string &fromDeviceId,
                                                 const std::string &audioId,
                                                 const std::string &url,
                                                 const std::string &name,
                                                 float position)
        { 
            return ResultCodeErrorNotSupport; 
        }

        /**
         * ���ûط��ٶ�
         *  @fromDeviceId   [in] ���ͷ����豸ID
         *  @url            [in] ��Ƶ����ַ
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @rate           [in] 0: ��ͣ��1: �������ʲ���
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode setPlayAudioRate     (const std::string &fromDeviceId,
                                                 const std::string &audioId,
                                                 float rate)
        { 
            return ResultCodeErrorNotSupport; 
        }

        /**
         * ֹͣ������Ƶ
         *  @toDeviceId     [in] ���շ����豸ID
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode stopPlayAudio        (const std::string &fromDeviceId,
                                                 const std::string &audioId)
        {
            return ResultCodeErrorNotSupport;
        }

        /**
         * ���ò��Ž���
         *  @toDeviceId     [in] ���շ����豸ID
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @playPosition   [in] ��ǰ������Ƶ��ʱ��㣬��λΪ��
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode setPlayAudioProgress  (const std::string &fromDeviceId,
                                                  const std::string &audioId,
                                                  float playPosition)
        {
            return ResultCodeErrorNotSupport;
        }

        /**
         * ��ȡ���Ž���
         *  @toDeviceId     [in] ���շ����豸ID
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @rate           [out] 0:��ͣ��1:����
         *  @position       [out] ��ǰ����/��ͣʱ��㣬��λΪ��
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode getPlayAudioProgress  (const std::string &fromDeviceId,
                                                  const std::string &audioId,
                                                  float *rate,
                                                  float *position)
        {
            return ResultCodeErrorNotSupport;
        }

        /**
         * ��ȡ������Ϣ
         *  @toDeviceId     [in] ���շ����豸ID
         *  @info           [out] ��Ƶ��Ϣ
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode getPlayAudioInfo       (const std::string &fromDeviceId,
                                                   PlayInfo *info)
        {
            return ResultCodeErrorNotSupport;
        }

        //------------------------------------------------------------------------------------
        // ϵͳ��ӿ�
        //------------------------------------------------------------------------------------

        /**
         * ��������
         *  @toDeviceId     [in] ���շ����豸ID
         *  @volume         [in] ��ǰ����(1-100)
         *  @return �ỰID
         */
        virtual ResultCode setVolume               (const std::string &fromDeviceId,
                                                    int volume)
        {
            return ResultCodeErrorNotSupport;
        }


        /**
         * ��ȡ����
         *  @toDeviceId     [in] ���շ����豸ID
         *  @volume         [out] ��ǰ����(1-100)
         *  @return �ỰID
         */
        virtual ResultCode getVolume               (const std::string &fromDeviceId,
                                                    int * volume)
        {
            return ResultCodeErrorNotSupport;
        }

        //------------------------------------------------------------------------------------
        // ��չ��Ϣ�ӿ�
        //------------------------------------------------------------------------------------     
        /**
         * ������չ��Ϣ
         *  @fromDeviceId   [in] ���ͷ����豸ID
         *  @appXmlns       [in] ���ֿռ�
         *  @appParam       [in] ����
         *  @appResult      [out] ���
         *  @return
         *     true  - �ɹ��������Ϣ
         *     false - �޷��������Ϣ
         */
        virtual bool handleMessage              (const std::string &fromDeviceId,
                                                 const std::string &appXmlns,
                                                 const std::string &appParam,
                                                 std::string &appResult)
        { 
            return false; 
        }
    };

}

#endif // __AIRJOY_SERVER_LISTENER_H__