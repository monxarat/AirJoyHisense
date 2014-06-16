/*
* AudioBuffer.hpp
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

#ifndef __AUDIO_BUFFER_H__
#define __AUDIO_BUFFER_H__

#include "TheBoost.hpp"

#include <boost/noncopyable.hpp>
#include <boost/utility/value_init.hpp>
#include <boost/assert.hpp>
#include <boost/concept_check.hpp>

#define BOOST_ALL_NO_LIB
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

namespace airjoy
{

    class AudioPlayer;
    class AirPort;

    class AudioBuffer
    {
    private:
        typedef boost::mutex                        MutexType;
        typedef MutexType::scoped_lock              LockType;
        typedef boost::condition_variable_any       ConditionType;

        typedef unsigned short                      SequenceType;

        typedef struct 
        {
            double hist[2];
            double a[2];
            double b[3];
        } biquad_t;

        // decoded audio packets
        typedef struct _bufferType
        {
            int ready;
            signed short *data;
        } BufferType;

    private:
        AirPort                *m_airport;
        AudioPlayer            *m_player;

        #define BUFFER_FRAMES	512
        BufferType              m_buffer[BUFFER_FRAMES];
        bool                    m_isBufferInit;
        MutexType               m_mutex;
        ConditionType           m_hasFrame;

        // mutex-protected variables
        SequenceType            m_read;     
        SequenceType            m_write;
        bool                    m_buffering;
        bool                    m_synced;
        int                     m_start_fill;

        // for buffer data
        double                  m_playback_rate;
        double                  m_est_drift;
        double                  m_est_err;
        double                  m_last_err;
        biquad_t                m_drift_lpf;
        biquad_t                m_err_lpf;
        biquad_t                m_err_deriv_lpf;
        double                  m_desired_fill;
        int                     m_fill_count;
        
        // for getFrame()
        bool                    m_putNullPacket;

    public:
        AudioBuffer(AirPort *airport, AudioPlayer *player);
        virtual ~AudioBuffer();

        void resync(void);

        short *getFrame(bool *quit);
        int stuffBuffer(short *inptr, short *outptr);

        void putPacket(SequenceType seqno, const char *data, int len);
        void putNullPacket(void);

    private:
        void estReset(short fill);
        void estUpdate(short fill);

        void biquad_init(biquad_t *bq, double a[], double b[]);
        void biquad_lpf(biquad_t *bq, double freq, double Q);
        double biquad_filt(biquad_t *bq, double in);

    private:
        void initBuffer(void);
        void destroyBuffer(void);

        void rtp_request_resend(SequenceType first, SequenceType last);

        int seqOrder(SequenceType a, SequenceType b);
        short dithered_vol(short sample);
    };


}


#endif // __AUDIO_BUFFER_H__
