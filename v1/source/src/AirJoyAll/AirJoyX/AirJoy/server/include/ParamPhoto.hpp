/*
* ParamPhoto.cpp
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

    class ParamPhoto : public HttpTextParam
    {
    private:
        std::string     m_ip;
        std::string     m_url;

    public:
        ParamPhoto() {}
        virtual ~ParamPhoto() {}

        bool load(const std::string &text, const std::string &peerIp)
        {
            m_ip = peerIp;
            
            this->loadBytes(text.c_str(), text.length());

            return true;
        }

        const std::string & photoId(void)
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

        PhotoActionType action(void)
        {
            const std::string & actionValue = getValueByKey("action");
            if (actionValue == "cache")
                return PhotoActionCache;

            return PhotoActionDisplay;
        }

        PhotoDirectionType direction(void)
        {
            const std::string & directionValue = getValueByKey("direction");

            int t = PhotoDirectionRight;

            try
            {
                t = boost::lexical_cast<int>(directionValue);
            }
            catch (boost::bad_lexical_cast &e)
            {
                std::cout << "error: " << e.what() << std::endl;
            }

            PhotoDirectionType type = (PhotoDirectionType)t;
            
            return type;
        }

        float rate(void)
        {
            const std::string & rateValue = getValueByKey("rate");

            float r = 0.0f;

            try
            {
                r = boost::lexical_cast<float>(rateValue);
            }
            catch (boost::bad_lexical_cast &e)
            {
                std::cout << "error: " << e.what() << std::endl;
            }

            return r;
        }

        float x(void)
        {
            const std::string & xValue = getValueByKey("x");

            float value = 0.0f;

            try
            {
                value = boost::lexical_cast<float>(xValue);
            }
            catch (boost::bad_lexical_cast &e)
            {
                std::cout << "error: " << e.what() << std::endl;
            }

            return value;
        }

        float y(void)
        {
            const std::string & yValue = getValueByKey("y");

            float value = 0.0f;

            try
            {
                value = boost::lexical_cast<float>(yValue);
            }
            catch (boost::bad_lexical_cast &e)
            {
                std::cout << "error: " << e.what() << std::endl;
            }

            return value;
        }
    };

}


#endif // __PARAM_PHOTO_H__
