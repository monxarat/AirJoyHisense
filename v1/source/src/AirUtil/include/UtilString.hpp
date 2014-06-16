/*
* UtilString.hpp
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
*
*/
        
#ifndef __UTIL_STRING_H__
#define __UTIL_STRING_H__

#include <string>

namespace airjoy
{

    namespace util
    {
        /*
         * ���ַ���buffer���\n\0�����һ�����ַ���
         *  @buffer             [in] ָ���ַ���
         *  @bufferSize         [in] �ַ�������
         *  @hasEnd             [in] \0
         *  @hasNewLine         [in] \n
         *  @newBuffer          [out] ���ַ���
         *  @return             ���ַ�����newBuffer��
         */
        char * getTrimmed(const char *buffer,
                          int bufferSize,
                          bool hasEnd,
                          bool hasNewLine, 
                          char *newBuffer);

        /*
         * ���ַ���buffer���\n\0������һ�����ַ���
         *  @buffer             [in] ָ���ַ���
         *  @bufferSize         [in] �ַ�������
         *  @hasEnd             [in] \0
         *  @hasNewLine         [in] \n
         *  @return             ���ַ�����newBuffer��
         *  @note               ���ַ�����Ҫ�ֶ��ͷ�
         */
        char *getTrimmedMalloc(const char *buffer, int bufferSize, bool hasEnd, bool hasNewLine);

        /*
         * �� buffer �� pField ֮�� pLenAfterField ��λ�ã���ʼ��ȡ�ַ�����ֱ������pDelims�ַ�����
         *  @pContent           [in] http content����
         *  @pField             [in] ָ���ַ���
         *  @value              [out] �������ַ���
         */
        const char *getFromBuffer(const char *pBuffer, const char *pField, int lenAfterField, int *pReturnSize, char *pDelims);
     
        /*
         * ��http content�ж�ȡһ���ַ������ַ���λ��pFieldλ��1���ֽ�֮��
         *  @pContent           [in] http content����
         *  @pField             [in] ָ���ַ���
         *  @value              [out] �������ַ���
         */
        size_t getValueFromContent(const char *pContent, const char* pField, std::string &value);
        
    }
}


#endif // __UTIL_STRING_H__
