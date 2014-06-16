/*
 * AirPlayClientPhoto.hpp
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

#ifndef __AIRPLAY_CLIENT_PHOTO_H__
#define __AIRPLAY_CLIENT_PHOTO_H__

#include <string>
#include "AnyPlayTypeDef.hpp"
#include "AnyPlayApi.hpp"

namespace anyplay
{

    class AirPlayClient;

    class ANYPLAY_API AirPlayClientPhoto
    {
    public:
        AirPlayClientPhoto(AirPlayClient *sender);
        virtual ~AirPlayClientPhoto();

        /**
         * ������Ƭ��
         *  @photoPath      [in] ��Ƭ��ַ
         *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ������������MD5����UUID
         *  @action         [in] ��ʾ(PhotoActionDisplay)���򻺴�(PhotoActionCache)
         *  @return
         *      true  - ���ͳɹ�
         *      false - ����ʧ��
         */
        bool displayPhoto                                  (const std::string &photoPath,
                                                            const std::string &photoId,
                                                            PhotoActionType action = PhotoActionDisplay);

        /**
         * ��ʾ�Ѿ��������Ƭ
         *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
         *  @return
         *      true  - ���ͳɹ�
         *      false - ����ʧ��
         */
        bool displayCachedPhoto                            (const std::string &photoId);

        /**
         * ֹͣ��ʾ��Ƭ
         *  @return
         *      true  - ���ͳɹ�
         *      false - ����ʧ��
         */
        bool stopDisplayPhoto                              (void);

    private:
        AirPlayClient  *m_sender;
    };

}

#endif // __AIRPLAY_CLIENT_PHOTO_H__
