/*
* coding.tom@gmail.com 
*
* UtilMd5.hpp
*
* 2011-5-21
*
* @remark
*      set tabstop=4
*      set shiftwidth=4
*      set expandtab
*/

#ifndef __UTIL_MD5_H__
#define __UTIL_MD5_H__

#include "AirJoyTypeDef.hpp"
#include <string>

namespace airjoy
{

    class UtilMd5
    {
    public:
        UtilMd5(const unsigned char *text);
        ~UtilMd5();

        const char * decrypt(void) { return m_code; }

    private:
        typedef struct _Md5Ctx
        {
            unsigned int    count[2];
            unsigned int    state[4];
            unsigned char   buffer[64];
        } Md5Ctx;

        void init(Md5Ctx *context);
        void update(Md5Ctx *context, const unsigned char *input, unsigned int inputlen);
        void final(Md5Ctx *context, unsigned char digest[16]);
        void transform(unsigned int state[4], const unsigned char block[64]);
        void encode(unsigned char *output, unsigned int *input, unsigned int len);
        void decode(unsigned int *output, const unsigned char *input, unsigned int len);

    private:
        Md5Ctx            m_ctx;
        unsigned char     m_decrypt[16];
        char              m_code[32 + 1];
    };

}

#endif // __UTIL_MD5_H__
