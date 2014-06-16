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
         * �����ļ�
         *  @return
         *     ResultCodeOk - �ɹ�
         *     ResultCodeErrorNotSupport - ��֧�ִ˹���
         */
        virtual ResultCode putFile           (void)
        {
            return ResultCodeErrorNotSupport; 
        }
    };

}


#endif // __AIR_FILE_LISTENER_H__

