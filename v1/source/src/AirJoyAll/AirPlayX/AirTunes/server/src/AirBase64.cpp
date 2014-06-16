/*
* coding.tom@gmail.com 
*
* AirBase64.cpp
*
* 2011-5-21
*
* @remark
*      set tabstop=4
*      set shiftwidth=4
*      set expandtab
*/

#include "AirBase64.hpp"

#include <openssl/bio.h>
#include <openssl/ssl.h>

namespace airjoy
{
namespace airbase64
{

char * decode_base64(unsigned char *pInput, int pLength, int *pActualLength)
{
    // Needs All NO_NL flags for proper RSA AES KEY Descrypt
    BIO *b64, *bmem;
    unsigned char *input = pInput;
    int length = getCorrectedEncodeSize(pLength);

    if (pLength != length)
    {
        input = (unsigned char *)malloc(length * sizeof(unsigned char));
        memset(input, 0, length);
        memcpy(input, pInput, pLength);
        memset(input+pLength, '=', length-pLength);
        //printf("Fixed value: [%.*s]\n", length, input);
    }

    char *buffer = (char *)malloc(length);
    memset(buffer, 0, length);

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    bmem = BIO_new_mem_buf(input, length);
    BIO_set_flags(bmem, BIO_FLAGS_BASE64_NO_NL);
    bmem = BIO_push(b64, bmem);

    BIO_set_flags(bmem, BIO_FLAGS_BASE64_NO_NL);

    *pActualLength = BIO_read(bmem, buffer, length);

    BIO_free_all(bmem);

    if(pLength != length)
    {
        free(input);
    }

    return buffer;
}

char * encode_base64(unsigned char *input, int length)
{
    BIO *bmem, *b64;
    BUF_MEM *bptr;

    b64 = BIO_new(BIO_f_base64());
    // This enables/disables nls
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bmem = BIO_new(BIO_s_mem());

    b64 = BIO_push(b64, bmem);

    BIO_write(b64, input, length);
    (void)BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);

    char *buff = (char *)malloc(bptr->length);
    memcpy(buff, bptr->data, bptr->length-1);
    buff[bptr->length-1] = 0;

    BIO_free_all(b64);

    return buff;
}

int getCorrectedEncodeSize(int pSize)
{
    if(pSize % 4 == 0)
    {
        return pSize;
    }
    else if((pSize + 1) % 4 == 0)
    {
        return pSize+1;
    }
    else if((pSize + 2) % 4 == 0)
    {
        return pSize+2;
    }
    else
    {
        // Invalid encoded data, no other cases are possible.
        //printf("Unrecoverable error....base64 values are incorrectly encoded\n");
        return pSize;
    }
}

} // namespace airbase64
} // namespace airjoy
