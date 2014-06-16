/*
 * AirMediaClientPhoto.hpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-15
 *
 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#ifndef __AIRMEDIA_CLIENT_PHOTO_H__
#define __AIRMEDIA_CLIENT_PHOTO_H__

#include <string>
#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"

namespace airjoy
{

    class AirMediaClient;

    class AIRJOY_API AirMediaClientPhoto
    {
    public:
        AirMediaClientPhoto(AirMediaClient *sender);
        virtual ~AirMediaClientPhoto();

        /**
         * ������Ƭ��
         *  @photoUrl       [in] ������Ƭ����ַ
         *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ������������MD5����UUID
         *  @action         [in] ��ʾ(PhotoActionDisplay)���򻺴�(PhotoActionCache)
         *  @photoNameUtf8  [in] ��Ƭ����(��ѡ)
         *  @return �ỰID
         */
        AirJoySessionId    displayPhoto                   (const std::string &photoUrl,
                                                           const std::string &photoId,
                                                           PhotoActionType action = PhotoActionDisplay,
                                                           const std::string &photoNameUtf8 = "");
        /**
         * ��ʾ�Ѿ��������Ƭ
         *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
         *  @return �ỰID
         */
        AirJoySessionId    displayCachedPhoto             (const std::string &photoId);

        /**
         * ֹͣ��ʾ��Ƭ
         *  @return �ỰID
         */
        AirJoySessionId    stopDisplayPhoto               (void);

        /**
         * ��ת��Ƭ
         *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
         *  @direction      [in] ��ת���򣬼�PhotoDirectionType
         *  @return �ỰID
         */
        AirJoySessionId    rotatePhoto                    (const std::string &photoId,
                                                           PhotoDirectionType direction);

        
        /**
         * ������Ƭ
         *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
         *  @rate           [in] ���ű���
         *                       rate = 0, ����������ʴ�С
         *                       rate = 1, ������ԭ��С
         *                       rate > 1, �Ŵ�
         *                       0 < rate < 1, ��С
         *  @return �ỰID
         */
        AirJoySessionId    zoomPhoto                      (const std::string &photoId,
                                                           float rate);

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
         *  @return �ỰID
         */
        AirJoySessionId    movePhoto                      (const std::string &photoId,
                                                           float x,
                                                           float y);

    private:
        AirMediaClient  *m_sender;
    };

}

#endif // __AIRMEDIA_CLIENT_PHOTO_H__
