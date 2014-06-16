/*
* ResultAudio.cpp
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

#ifndef __RESULT_AUDIO_H__
#define __RESULT_AUDIO_H__

#include <string>
#include "AirJoyTypeDef.hpp"
#include "HttpTextParam.hpp"
#include "TheBoost.hpp"

namespace airjoy
{

    class ResultAudio : public HttpTextParam
    {
    public:
        ResultAudio() {}
        virtual ~ResultAudio() {}

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

        const std::string & audioId(void)
        {
            return getValueByKey("id");
        }

        const std::string & url(void)
        {
            return getValueByKey("url");
        }

        const std::string & name(void)
        {
            return getValueByKey("name");
        }

        float position(void)
        {
            const std::string & value = getValueByKey("position");

            float v = 0.0f;

            try
            {
                v = boost::lexical_cast<float>(value);
            }
            catch (boost::bad_lexical_cast &e)
            {
                std::cout << "error: " << e.what() << std::endl;
            }

            return v;
        }

        float duration(void)
        {
            const std::string & value = getValueByKey("duration");

            float v = 0.0f;

            try
            {
                v = boost::lexical_cast<float>(value);
            }
            catch (boost::bad_lexical_cast &e)
            {
                std::cout << "error: " << e.what() << std::endl;
            }

            return v;
        }

        float rate(void)
        {
            const std::string & value = getValueByKey("rate");

            float v = 0.0f;

            try
            {
                v = boost::lexical_cast<float>(value);
            }
            catch (boost::bad_lexical_cast &e)
            {
                std::cout << "error: " << e.what() << std::endl;
            }

            return v;
        }
    };

}


#endif // __RESULT_AUDIO_H__
