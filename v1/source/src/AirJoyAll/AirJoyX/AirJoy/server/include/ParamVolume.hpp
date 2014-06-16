/*
* ParamSystem.cpp
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

#ifndef __PARAM_SYSTEM_H__
#define __PARAM_SYSTEM_H__

#include <string>
#include "AirJoyTypeDef.hpp"
#include "HttpTextParam.hpp"
#include "TheBoost.hpp"

namespace airjoy
{

    class ParamVolume : public HttpTextParam
    {
    public:
        ParamVolume() {}
        virtual ~ParamVolume() {}

        bool load(const std::string &text)
        {
            this->loadBytes(text.c_str(), text.length());

            return true;
        }

        int value(void)
        {
            const std::string & stringValue = getValueByKey("value");

            int v = 0;

            try
            {
                v = boost::lexical_cast<int>(stringValue);
            }
            catch (boost::bad_lexical_cast &e)
            {
                std::cout << "error: " << e.what() << std::endl;
            }

            return v;
        }
    };

}


#endif // __PARAM_SYSTEM_H__
