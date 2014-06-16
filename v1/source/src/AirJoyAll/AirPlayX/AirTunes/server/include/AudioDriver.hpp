/*
* AudioDriver.hpp
*
*
* coding.tom@gmail.com
*
* 2013-2-4
*
* @remark
*      set tabstop=4
*      set shiftwidth=4
*      set expandtab
*/

#ifndef __AUDIO_DRIVER_H__
#define __AUDIO_DRIVER_H__

#include "TheBoost.hpp"

#include <string>

#ifdef __USE_LIB_AO__
#include <ao/ao.h>
#endif // __USE_LIB_AO__

namespace airjoy
{
	class AirTunesServerListener;

    #ifdef __USE_LIB_AO__

    class AudioDriver
    {
    private:
        ao_device               *m_dev;
        
    #ifdef _WIN32
        std::string              m_file;
        HANDLE                   m_handle;
    #endif

        int                      m_samplingRate;
        int                      m_fmtp[12];

    public:
        AudioDriver(AirTunesServerListener *listener);
        virtual ~AudioDriver();

        void setSamplingRate(int samplingRate);
        void setFmtp(int fmtp[12], int len);

    #ifdef _WIN32
        void setOutputFile(char *file);
        void setOutputHandle(HANDLE handle);
    #endif
        
        // must called after setSamplingRate()
        void init(void);
        void play(char *outbuf, int playSamples);

    private:
        void init(char *libao_driver);
    };
 
    #else

    class AudioDriver
    {
    private:
        AirTunesServerListener  *m_listener;
        int                      m_samplingRate;
        int                      m_fmtp[12];

    public:
        AudioDriver(AirTunesServerListener *listener);
        virtual ~AudioDriver();

        void setSamplingRate(int samplingRate);
        void setFmtp(int fmtp[12], int len);

        // must called after setSamplingRate()
        void init(void);
        
        void play(char *outbuf, int playSamples);
    };

    #endif // __USE_LIB_AO__

}


#endif // __AO_DRIVER_H__
