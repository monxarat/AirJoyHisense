/*
* UmpMessage.h
*
* Universal Message Procotol
*
* coding.tom@gmail.com
*
* 2012-2-29
*
* @remark
*      set tabstop=4
*      set shiftwidth=4
*      set expandtab
*/

#ifndef __UMP_MESSAGE_H__
#define __UMP_MESSAGE_H__

#include <string>

#include <rapidxml/rapidxml.hpp>

namespace airjoy
{

    /*
    <iq type="get" id="001" from="fox" to="alex">
        <query xmlns="http://www.airjoy.cn/ump/query">
            <application action="http://www.airjoy.cn/query/getsharedfolder">
                <param>$PARAM$</param>      // base64 encode
            </application>
        </query>
    </iq>

    <iq type="result" id="001" from="fox" to="alex">
        <query xmlns="http://www.airjoy.cn/ump/query">
            <application action="http://www.airjoy.cn/query/getsharedfolder">
                <result>$RESULT$</result>   // base64 encode
            </application>
        </query>
    </iq>

    <iq type="error" id="001" from="fox" to="alex">
        <query xmlns="http://www.airjoy.cn/ump/query">
            <application action="http://www.airjoy.cn/query/getsharedfolder">
                <result>$RESULT$</result>   // base64 encode
            </application>
        </query>
    </iq>
    */

    typedef enum _UmpMessageType
    {
        UmpMessageTypeGet     = 0,
        UmpMessageTypeSet     = 1,
        UmpMessageTypeResult  = 2,
        UmpMessageTypeError   = 3,
        UmpMessageTypeInvalid,
    } UmpMessageType;

    class UmpMessage
    {
    public:
        UmpMessage();
        UmpMessage(UmpMessageType type, 
            const std::string &id, 
            const std::string &from, 
            const std::string &to);
        virtual ~UmpMessage();
        
        UmpMessage & operator=(UmpMessage &msg);

        void setFrom(const std::string &from) { m_from = from; }
        const std::string & from(void) {return m_from; }

        void setTo(const std::string &to) { m_to = to; }
        const std::string & to(void) {return m_to; }

        void setId(int id);
        void setId(const std::string &id) { m_id = id; }
        const std::string & id(void) { return m_id; } 
        int idInteger(void) { return atoi(m_id.c_str()); }

        void setXmlns(const std::string &xmlns) { m_xmlns = xmlns; }
        const std::string & xmlns(void) { return m_xmlns; } 

        void setAppAction(const std::string &action) { m_appAction = action; }
        const std::string & appAction(void) { return m_appAction; } 

        void setAppParam(const std::string &param) { m_appParam = param; }
        const std::string & appParam(void) { return m_appParam; } 

        void setAppResult(const std::string &result) { m_appResult = result; }
        const std::string & appResult(void) { return m_appResult; } 

        void setType(UmpMessageType type) { m_type = type; }
        UmpMessageType type(void) { return m_type; }
    
        void setFromIp(const std::string &ip) { m_fromIp = ip; }
        const std::string & fromIp(void) { return m_fromIp; }

        bool loadText(const char *data, int length);
        bool loadText(const std::string &text);
        const char * toText(void);
        const std::string & toString(void);

    private:
        static UmpMessageType textToMessageType(const char *text);
        static const char * messageTypeToText(UmpMessageType type);

    private:
        std::string         m_fromIp;

        std::string         m_from;
        std::string         m_to;
        std::string         m_id;
        std::string         m_xmlns;
        std::string         m_appAction;
        std::string         m_appParam;
        std::string         m_appResult;
        UmpMessageType      m_type;

    private:
        rapidxml::xml_document<> *m_doc;
        rapidxml::xml_node<>     *m_iq;
        rapidxml::xml_node<>     *m_iqQuery;
        rapidxml::xml_node<>     *m_iqQueryApp;
        rapidxml::xml_node<>     *m_iqQueryAppParam;
        rapidxml::xml_node<>     *m_iqQueryAppResult;
        std::string               m_buffer;
    };

}

#endif // __UMP_MESSAGE_H__
