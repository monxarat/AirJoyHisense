/*
* coding.tom@gmail.com 
*
* AirBase64.h
*
* 2011-5-21
*
* @remark
*      set tabstop=4
*      set shiftwidth=4
*      set expandtab
*/

#ifndef __AIR_BASE64_H__
#define __AIR_BASE64_H__


namespace airjoy
{

    namespace airbase64
    {
        // All calls to decode and encode need to be freed
        char *decode_base64(unsigned char *input, int length, int *tActualLength);

        // All calls to decode and encode need to be freed
        char *encode_base64(unsigned char *input, int length);

        // for decode_base64
        int getCorrectedEncodeSize(int pSize);
    }

}

#endif // __AIR_BASE64_H__

