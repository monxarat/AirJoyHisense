/*
* coding.tom@gmail.com
* 
* UtilItoa.cpp
*
* 2011-5-14
*
* @remark
*      set tabstop=4
*      set shiftwidth=4
*      set expandtab
*/

#include "UtilItoa.hpp"

namespace airjoy
{

#if defined(_WIN32) || defined(_WIN64)

#else

    errno_t _itoa_s (int value, char *buffer, size_t sizeInCharacters, int radix)
    {
        int len = 0;
        int power = 0;
        int j = 0;

        j = value; 

        for (power=1; j>=radix; j/=radix) 
            power *= radix; 

        for (; power>0; power/=radix)
        {
            *buffer++ = '0' + value/power; 
            value %= power; 

            len ++;
            if ((sizeInCharacters - len) == 1)
                break;
        }

        *buffer = '\0';

        return 0;
    }
     
#endif

} // namespace airjoy