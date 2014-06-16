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
         * 将字符串buffer添加\n\0，变成一个新字符串
         *  @buffer             [in] 指定字符串
         *  @bufferSize         [in] 字符串长度
         *  @hasEnd             [in] \0
         *  @hasNewLine         [in] \n
         *  @newBuffer          [out] 新字符串
         *  @return             新字符串（newBuffer）
         */
        char * getTrimmed(const char *buffer,
                          int bufferSize,
                          bool hasEnd,
                          bool hasNewLine, 
                          char *newBuffer);

        /*
         * 将字符串buffer添加\n\0，生成一个新字符串
         *  @buffer             [in] 指定字符串
         *  @bufferSize         [in] 字符串长度
         *  @hasEnd             [in] \0
         *  @hasNewLine         [in] \n
         *  @return             新字符串（newBuffer）
         *  @note               新字符串需要手动释放
         */
        char *getTrimmedMalloc(const char *buffer, int bufferSize, bool hasEnd, bool hasNewLine);

        /*
         * 从 buffer 中 pField 之后 pLenAfterField 个位置，开始读取字符串，直到遇到pDelims字符串。
         *  @pContent           [in] http content数据
         *  @pField             [in] 指定字符串
         *  @value              [out] 读出的字符串
         */
        const char *getFromBuffer(const char *pBuffer, const char *pField, int lenAfterField, int *pReturnSize, char *pDelims);
     
        /*
         * 从http content中读取一段字符串，字符串位于pField位置1个字节之后
         *  @pContent           [in] http content数据
         *  @pField             [in] 指定字符串
         *  @value              [out] 读出的字符串
         */
        size_t getValueFromContent(const char *pContent, const char* pField, std::string &value);
        
    }
}


#endif // __UTIL_STRING_H__
