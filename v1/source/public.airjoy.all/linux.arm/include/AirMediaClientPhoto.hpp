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
         * 发送照片流
         *  @photoUrl       [in] 网络照片流地址
         *  @photoId        [in] 标识照片的唯一字符串，建议采用MD5，或UUID
         *  @action         [in] 显示(PhotoActionDisplay)，或缓存(PhotoActionCache)
         *  @photoNameUtf8  [in] 照片名称(可选)
         *  @return 会话ID
         */
        AirJoySessionId    displayPhoto                   (const std::string &photoUrl,
                                                           const std::string &photoId,
                                                           PhotoActionType action = PhotoActionDisplay,
                                                           const std::string &photoNameUtf8 = "");
        /**
         * 显示已经缓存的照片
         *  @photoId        [in] 标识照片的唯一字符串
         *  @return 会话ID
         */
        AirJoySessionId    displayCachedPhoto             (const std::string &photoId);

        /**
         * 停止显示照片
         *  @return 会话ID
         */
        AirJoySessionId    stopDisplayPhoto               (void);

        /**
         * 旋转照片
         *  @photoId        [in] 标识照片的唯一字符串
         *  @direction      [in] 旋转方向，见PhotoDirectionType
         *  @return 会话ID
         */
        AirJoySessionId    rotatePhoto                    (const std::string &photoId,
                                                           PhotoDirectionType direction);

        
        /**
         * 缩放照片
         *  @photoId        [in] 标识照片的唯一字符串
         *  @rate           [in] 缩放比例
         *                       rate = 0, 缩放至最合适大小
         *                       rate = 1, 缩放至原大小
         *                       rate > 1, 放大
         *                       0 < rate < 1, 缩小
         *  @return 会话ID
         */
        AirJoySessionId    zoomPhoto                      (const std::string &photoId,
                                                           float rate);

        /**
         * 移动照片
         *  @photoId        [in] 标识照片的唯一字符串
         *  @x              [in] x轴移动比例，有符号浮点数，单位：接收方屏幕宽度
　       *                       x值域范围：(-1 < x < 1) 
　       *                       x = 0: 不移动
　       *                       0 < x < 1: 向右移动
　       *                       　-1 < x < 0: 向左移动
　       *                       假如接收方屏幕宽度为800，则x = 0.3，代表图片向屏幕右方移动（0.3 x 800）个像素点。
　       *  @y             [ in] y轴移动比例，有符号浮点数，单位：接收方屏幕高度
　       *                     　y值域范围：(-1 < y < 1) 
　       *                     　y = 0: 不移动
　       *                       0 < y < 1: 向下移动
　       *                     　-1 < y < 0: 向上移动
　       *                     　假如接收方屏幕宽度为600，则y = 0.3，代表图片向屏幕下方移动（0.3 x 600）个像素点。
         *  @return 会话ID
         */
        AirJoySessionId    movePhoto                      (const std::string &photoId,
                                                           float x,
                                                           float y);

    private:
        AirMediaClient  *m_sender;
    };

}

#endif // __AIRMEDIA_CLIENT_PHOTO_H__
