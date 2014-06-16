/*
* ResultPhoto.cpp
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

#ifndef __PARAM_PHOTO_H__
#define __PARAM_PHOTO_H__

#include <string>
#include "AirJoyTypeDef.hpp"
#include "HttpTextParam.hpp"
#include "TheBoost.hpp"

namespace airjoy
{

    class ResultPhoto : public HttpTextParam
    {
    public:
        ResultPhoto() {}
        virtual ~ResultPhoto() {}

        bool load(const std::string &text)
        {
            this->loadBytes(text.c_str(), text.length());

            return true;
        }

        ResultCode code(void)
        {
            const std::string & stringValue = getValueByKey("code");

            int v = 0;

            try
            {
                v = boost::lexical_cast<int>(stringValue);
            }
            catch (boost::bad_lexical_cast &e)
            {
                std::cout << "error: " << e.what() << std::endl;
            }

            return (ResultCode)v;
        }
    };

}


#endif // __PARAM_PHOTO_H__
