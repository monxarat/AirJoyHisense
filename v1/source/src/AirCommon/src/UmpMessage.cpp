/*
* UmpMessage.cpp
*
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

#include "UmpMessage.hpp"
#include <string.h>
#include <sstream>

#include "rapidxml/rapidxml_print.hpp"

using namespace rapidxml;
using namespace airjoy;

//---------------------------------------------------------------------------------------------
// Public API
//---------------------------------------------------------------------------------------------

UmpMessage::UmpMessage()
{

    /*
    <iq type="get" id="001" from="fox" to="alex">
        <query xmlns="http://www.airjoy.cn/query">
            <application action="http://www.airjoy.cn/query/getsharedfolder">
                <param>$PARAM$</param>
            </application>
        </query>
    </iq>
    */

    m_doc = new rapidxml::xml_document<>;
    m_iq = m_doc->allocate_node(node_element, "iq");
    m_iqQuery = m_doc->allocate_node(node_element, "query");
    m_iqQueryApp = m_doc->allocate_node(node_element, "application");
    m_iqQueryAppParam = m_doc->allocate_node(node_element, "param");
    m_iqQueryAppResult = m_doc->allocate_node(node_element, "result");
}

UmpMessage::UmpMessage(UmpMessageType type, 
    const std::string &id, 
    const std::string &from, 
    const std::string &to)
{
    m_doc = new rapidxml::xml_document<>;
    m_iq = m_doc->allocate_node(node_element, "iq");
    m_iqQuery = m_doc->allocate_node(node_element, "query");
    m_iqQueryApp = m_doc->allocate_node(node_element, "application");
    m_iqQueryAppParam = m_doc->allocate_node(node_element, "param");
    m_iqQueryAppResult = m_doc->allocate_node(node_element, "result");

    m_type = type;
    m_id = id;
    m_from = from;
    m_to = to;
}

UmpMessage::~UmpMessage()
{
    delete m_doc;
}

UmpMessage & UmpMessage::operator=(UmpMessage &msg)
{
    m_fromIp = msg.fromIp();
    m_from = msg.from();
    m_to = msg.to();
    m_id = msg.id();
    m_xmlns = msg.xmlns();
    m_appAction = msg.appAction();
    m_appParam = msg.appParam();
    m_appResult = msg.appResult();
    m_type = msg.type();

    return *this;
}

void UmpMessage::setId(int id)
{
    std::stringstream ssId;
    ssId << id;
    m_id = ssId.str();
}

bool UmpMessage::loadText(const std::string &text)
{
    return this->loadText(text.c_str(), (int)(text.size()));
}

bool UmpMessage::loadText(const char *data, int length)
{
    xml_document<> doc;

    try
    {
        doc.parse<parse_default>((char *)data);
    }
    catch (std::exception &e)
    {
        (void)e;
        return false;
    }

    // <iq>
    xml_node<> *iq = doc.first_node();
    if (iq == NULL)
        return false;

    if (strcmp(iq->name(), "iq") != 0)
        return false;

    xml_attribute<char> * iqType = iq->first_attribute("type");
    xml_attribute<char> * iqId = iq->first_attribute("id");
    xml_attribute<char> * iqFrom = iq->first_attribute("from");
    xml_attribute<char> * iqTo = iq->first_attribute("to");

    if (iqType == NULL)
        return false;

    if (iqId == NULL)
        return false;

    if (iqFrom == NULL)
        return false;

    if (iqTo == NULL)
        return false;

    m_type = textToMessageType(iqType->value());
    m_id = iqId->value();
    m_from = iqFrom->value();
    m_to = iqTo->value();

    // <query>
    xml_node<>* query = iq->first_node("query");
    if (query == NULL)
        return false;

    xml_attribute<char> * queryXmlns = query->first_attribute("xmlns");
    if (queryXmlns == NULL)
        return false;

    m_xmlns = queryXmlns->value();

    // <application>
    xml_node<>* app = query->first_node("application");
    if (app == NULL)
        return false;

    xml_attribute<char> * appAction = app->first_attribute("action");
    if (appAction == NULL)
        return false;

    m_appAction = appAction->value();

    // <param> or <result>
    xml_node<>* appParam = app->first_node("param");
    if (appParam)
        m_appParam = appParam->value();
    else
    {
        xml_node<>* appResult = app->first_node("result");
        if (appResult)
            m_appResult = appResult->value();
    }

    return true;
}

const char * UmpMessage::toText(void)
{
    return this->toString().c_str();
}

const std::string & UmpMessage::toString(void)
{
    // 2012-12-14, jxfengzi@gmail.com
    // 不知为何，调用太快的时候，这里转换成xml字符串，会出错！
#if 0
    m_iq->remove_all_attributes();
    m_iq->append_attribute(m_doc->allocate_attribute("type", messageTypeToText(m_type)));
    m_iq->append_attribute(m_doc->allocate_attribute("id", m_id.c_str()));
    m_iq->append_attribute(m_doc->allocate_attribute("from", m_from.c_str()));
    m_iq->append_attribute(m_doc->allocate_attribute("to", m_to.c_str()));

    m_iqQuery->remove_all_attributes();
    m_iqQuery->append_attribute(m_doc->allocate_attribute("xmlns", m_xmlns.c_str()));

    m_iqQueryApp->append_attribute(m_doc->allocate_attribute("action", m_appAction.c_str()));

    m_doc->append_node(m_iq);
    m_iq->append_node(m_iqQuery);
    m_iqQuery->append_node(m_iqQueryApp);

    if (m_type == UmpMessageTypeResult || m_type == UmpMessageTypeError)
    {
        m_iqQueryApp->append_node(m_iqQueryAppResult);
        m_iqQueryAppResult->value(m_appResult.c_str());  
    }
    else
    {
        m_iqQueryApp->append_node(m_iqQueryAppParam);
        m_iqQueryAppParam->value(m_appParam.c_str());
    }

    m_buffer.clear();
    print(std::back_inserter(m_buffer), *m_doc, 0);
#else
    m_buffer.clear();
    m_buffer.append("<iq type=\"").append(messageTypeToText(m_type)).append("\" id=\"").append(m_id).append("\" from=\"").append(m_from).append("\" to=\"").append(m_to).append("\">");
    m_buffer.append("<query xmlns=\"").append(m_xmlns).append("\">");
    m_buffer.append("<application action=\"").append(m_appAction).append("\">");
    
    if (m_type == UmpMessageTypeResult || m_type == UmpMessageTypeError)
        m_buffer.append("<result>").append(m_appResult).append("</result>");
    else
        m_buffer.append("<param>").append(m_appParam).append("</param>");

    m_buffer.append("</application>");
    m_buffer.append("</query>");
    m_buffer.append("</iq>");
#endif

    return m_buffer;
}

//---------------------------------------------------------------------------------------------
// Private API
//---------------------------------------------------------------------------------------------

UmpMessageType UmpMessage::textToMessageType(const char *text)
{
    if (strcmp(text, "get") == 0)
        return UmpMessageTypeGet;

    if (strcmp(text, "set") == 0)
        return UmpMessageTypeSet;

    if (strcmp(text, "result") == 0)
        return UmpMessageTypeResult;

    if (strcmp(text, "error") == 0)
        return UmpMessageTypeError;

    return UmpMessageTypeInvalid;
}

const char * UmpMessage::messageTypeToText(UmpMessageType type)
{
    switch (type)
    {
    case UmpMessageTypeGet:
        return "get";

    case UmpMessageTypeSet:
        return "set";

    case UmpMessageTypeResult:
        return "result";

    case UmpMessageTypeError:
        return "error";

    default:
        return NULL;
    }

    return NULL;
}
