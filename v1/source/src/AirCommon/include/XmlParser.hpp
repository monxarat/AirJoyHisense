/*
* XmlParser.hpp
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

#ifndef __XML_PARSER_H__
#define __XML_PARSER_H__

#include <rapidxml/rapidxml.hpp>
#include <string>
#include <vector>
#include <map>

namespace airjoy
{

    class XmlParser
    {
    private:
        typedef std::map<std::string, std::string> KeyValueType;

    private:
        KeyValueType    m_keyValues;
        std::string     m_emptyValue;

    public:
        XmlParser() {}
        virtual ~XmlParser() {}

        bool parse(const std::string &xml, const std::string &rootName)
        {
            using namespace rapidxml;

            std::vector<char> xml_copy(xml.begin(), xml.end());
            xml_copy.push_back('\0');

            xml_document<> doc;
            
            try
            {
                doc.parse<parse_default>(&xml_copy[0]);
            }
            catch (std::exception &e)
            {
                (void)e;
                return false;
            }

            xml_node<> *root = doc.first_node();
            if (root == NULL)
                return false;

            if (rootName.compare(root->name()) != 0)
                return false;

            for(xml_node<> * node = root->first_node(); node != NULL; node = node->next_sibling())
                m_keyValues.insert(std::make_pair(node->name(), node->value()));
            
            return true;
        }

        const std::string & getValue(const std::string &key)
        {
            KeyValueType::iterator pos = m_keyValues.find(key);
            if (pos != m_keyValues.end())
                return pos->second;

            return m_emptyValue;
        }

        const std::string & getValue(const char *key)
        {
            std::string k(key);
            return getValue(k);
        }
    };

}


#endif // __XML_PARSER_H__

