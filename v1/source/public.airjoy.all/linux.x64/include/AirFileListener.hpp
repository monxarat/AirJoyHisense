/*
* AirFileListener.hpp
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

#ifndef __AIR_FILE_LISTENER_H__
#define __AIR_FILE_LISTENER_H__

#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"


namespace airjoy
{

    class AIRJOY_API AirFileListener
    {
    public:
        AirFileListener() {}
        virtual ~AirFileListener() {}

        /**
         * 推送文件
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode putFile           (void)
        {
            return ResultCodeErrorNotSupport; 
        }
    };

}


#endif // __AIR_FILE_LISTENER_H__

