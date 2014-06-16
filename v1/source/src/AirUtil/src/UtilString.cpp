/*
* coding.tom@gmail.com 
*
* UtilString.cpp
*
* 2011-5-21
*
* @remark
*      set tabstop=4
*      set shiftwidth=4
*      set expandtab
*/

#include "UtilString.hpp"
#include <string.h>
#include <stdlib.h>


namespace airjoy
{

namespace util
{


char * getTrimmed(const char *buffer,
                  int bufferSize,
                  bool hasEnd, 
                  bool hasNewLine, 
                  char *newBuffer)
{
    int tSize = bufferSize;
    if (hasEnd)
    {
        tSize++;
    }
    if(hasNewLine)
    {
        tSize++;
    }

    memset((void *)newBuffer, 0, tSize);
    memcpy((void *)newBuffer, buffer, bufferSize);
    
    if (hasNewLine)
    {
        newBuffer[bufferSize] = '\n';
    }

    if (hasEnd)
    {
        newBuffer[tSize-1] = '\0';
    }

    return newBuffer;
}

char *getTrimmedMalloc(const char *buffer, int bufferSize, bool hasEnd, bool hasNewLine)
{
    int tAdditionalSize = 0;

    if (hasEnd)
        tAdditionalSize++;
    
    if (hasNewLine)
        tAdditionalSize++;
    
    char *tTrimDest = (char *)malloc(sizeof(char) * (bufferSize + tAdditionalSize));
    
    return getTrimmed(buffer, bufferSize, hasEnd, hasNewLine, tTrimDest);
}

const char * getFromBuffer(const char *pBuffer, const char *pField, int lenAfterField, int *pReturnSize, char *pDelims)
{
    int tSize = 0;
    const char* tFound = strstr(pBuffer, pField);

    if (tFound != NULL)
    {
        tFound += (strlen(pField) + lenAfterField);
        
        int tIdx = 0;
        char tDelim = pDelims[tIdx];
        const char *tShortest = NULL;
        const char *tEnd = NULL;
        while (tDelim != '\0')
        {
            tDelim = pDelims[tIdx++]; // Ensures that \0 is also searched.
            tEnd = strchr(tFound, tDelim);
            if(tEnd != NULL && (NULL == tShortest || tEnd < tShortest))
            {
                tShortest = tEnd;
            }
        }

        tSize = (int) (tShortest - tFound);
        
        if(pReturnSize != NULL)
        {
            *pReturnSize = tSize;
        }
    }

    return tFound;
}

size_t getValueFromContent(const char *pContent, const char* pField, std::string &value)
{
    int size = 0;
    const char *v = getFromBuffer(pContent, pField, 1, &size, "\r\n");
    //const char *v = getFromBuffer(pContent, pField, 1, &size, "\n");
    if (v != NULL)
        value.append(v);
    
    return size;
}

} // namespace util

} // namespace airjoy
