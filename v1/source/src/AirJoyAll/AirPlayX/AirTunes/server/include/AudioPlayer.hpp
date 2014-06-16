/*
* AudioPlayer.hpp
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

#ifndef __AUDIO_PLAYER_H__
#define __AUDIO_PLAYER_H__

#include "TheBoost.hpp"
#include "alac.h"

#include <openssl/aes.h>

namespace airjoy
{
    
    class AirTunesServerListener;

    class AudioBuffer;
    class AudioDriver;

    class AudioPlayer : public boost::enable_shared_from_this<AudioPlayer>
    {
    public:
        typedef boost::shared_ptr<boost::thread>                ThreadPointer;
        typedef boost::shared_ptr<AudioDriver>                  AudioDriverPointer;

    private:
        AirTunesServerListener *m_listener;

        AudioBuffer            *m_buffer;
        AudioDriverPointer      m_driver;

        ThreadPointer           m_pThread;
        bool                    m_isStart;
        double                  m_bufferPlaybackRate;

        int                     m_frameSize;
        int                     m_samplingRate;

        double                  m_volume;
        long                    m_fixVolume;

        alac_file              *m_decoder;

        int                     m_fmtp[12];
        unsigned char           m_aeskey[16];
        unsigned char           m_aesiv[16];
        AES_KEY                 m_aes;
        bool                    m_isInit;

    public:
        AudioPlayer(AirTunesServerListener *listener);
        virtual ~AudioPlayer();

        void setBuffer(AudioBuffer *pBuffer);
        
        void setFmtp(int fmtp[12], int len);
        void setAesIV(unsigned char aesiv[16], int len);
        void setAesKey(unsigned char aesKey[16], int len);

        // initDecoder must called after setFmtp() & setAesIV() & setAesKey()
        bool initDecoder(void);

        bool start(void);
        bool stop(void);

        bool isStopped(void);

        int getFrameSize(void);
        int getSamplingRate(void);

        long fixVolume(void);

        void flushDecoder(void);
        void volumeDecoder(double lfVolume);

        void alacDecode(short *dest, const char *buf, int len);

    private:
        void thread(void);
        void destroyDecoder(void);
    };

}

#endif // __AUDIO_PLAYER_H__
