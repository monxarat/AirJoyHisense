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
         * 发送照片流
         *  @photoPath      [in] 照片地址
         *  @photoId        [in] 标识照片的唯一字符串，建议采用MD5，或UUID
         *  @action         [in] 显示(PhotoActionDisplay)，或缓存(PhotoActionCache)
         *  @return
         *      true  - 发送成功
         *      false - 发送失败
         */
        bool displayPhoto                                  (const std::string &photoPath,
                                                            const std::string &photoId,
                                                            PhotoActionType action = PhotoActionDisplay);

        /**
         * 显示已经缓存的照片
         *  @photoId        [in] 标识照片的唯一字符串
         *  @return
         *      true  - 发送成功
         *      false - 发送失败
         */
        bool displayCachedPhoto                            (const std::string &photoId);

        /**
         * 停止显示照片
         *  @return
         *      true  - 发送成功
         *      false - 发送失败
         */
        bool stopDisplayPhoto                              (void);

    private:
        AirPlayClient  *m_sender;
    };

}

#endif // __AIRPLAY_CLIENT_PHOTO_H__
