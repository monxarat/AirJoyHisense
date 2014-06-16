/*
* ParamAudio.cpp
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

#ifndef __PARAM_AUDIO_H__
#define __PARAM_AUDIO_H__

#include <string>
#include "AirJoyTypeDef.hpp"
#include "HttpTextParam.hpp"
#include "TheBoost.hpp"

namespace airjoy
{

    class ParamAudio : public HttpTextParam
    {
    private:
        std::string     m_ip;
        std::string     m_url;

    public:
        ParamAudio() {}
        virtual ~ParamAudio() {}

        bool load(const std::string &text, const std::string &peerIp)
        {
            m_ip = peerIp;
            
            this->loadBytes(text.c_str(), text.length());

            return true;
        }

        const std::string & audioId(void)
        {
            return getValueByKey("id");
        }

        const std::string & url(void)
        {
            m_url = getValueByKey("url");

            if (m_url.find("http://") == std::string::npos)
            {
                m_url.clear();
                m_url.append("http://");
                m_url.append(m_ip);
                m_url.append(getValueByKey("url"));
            }
            
            return m_url;
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


#endif // __PARAM_AUDIO_H__
