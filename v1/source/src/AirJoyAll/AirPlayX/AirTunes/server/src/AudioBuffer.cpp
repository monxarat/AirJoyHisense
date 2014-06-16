/*
* AudioBuffer.cpp
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

#include "AudioBuffer.hpp"
#include "AudioPlayer.hpp"
#include "AirPort.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace airjoy;

//#define BUFFER_START_FILL   282
#define BUFFER_START_FILL   64
#define BUFIDX(seqno) ((SequenceType)(seqno) % BUFFER_FRAMES)

static short lcg_rand(void) 
{ 
	static unsigned long lcg_prev = 12345; 

	lcg_prev = lcg_prev * 69069 + 3; 
	return lcg_prev & 0xffff; 
} 

AudioBuffer::AudioBuffer(AirPort *airport, AudioPlayer *player)
    : m_airport(airport)
    , m_player(player)
    , m_isBufferInit(false)
    , m_read(0)
    , m_write(0)
    , m_buffering(true)
    , m_synced(false)
    , m_start_fill(BUFFER_START_FILL)
    , m_playback_rate(1.0)
    , m_est_drift(0.0)
    , m_est_err(0.0)
    , m_last_err(0.0)
    , m_putNullPacket(false)
{
    //std::cout << "AudioBuffer::AudioBuffer()" << std::endl;

    this->initBuffer();
}

AudioBuffer::~AudioBuffer()
{
    //std::cout << "AudioBuffer::~AudioBuffer()" << std::endl;

    this->destroyBuffer();
}

void AudioBuffer::resync(void)
{
    int i;

    for (i=0; i < BUFFER_FRAMES; i++)
        m_buffer[i].ready = 0;

    m_synced = false;
}

short * AudioBuffer::getFrame(bool *quit)
{
    //std::cout << "AudioBuffer::getFrame()" << std::endl;

    short buf_fill;
    SequenceType read;

    // lock m_write & m_read
    {
        LockType lock(m_mutex);
        buf_fill = m_write - m_read;
    }

    if (buf_fill < 1 || ! m_synced) 
    {    
        // init or underrun. stop and wait
        if (m_synced)
        {
            //std::cout << "underrun." << std::endl;
        }

        m_buffering = true;

        // 等待条件变量：等待buffer准备完成
#if 0
        LockType lock(m_mutex);
        while (! m_player->isStopped())
            m_hasFrame.wait(m_mutex);
#else
        LockType lock(m_mutex);
        m_hasFrame.wait(m_mutex);
#endif
        // 用来中断调用主线程
        if (m_putNullPacket)
        {
            *quit = true;
            return NULL;
        }

		m_read++;
        buf_fill = m_write - m_read;

        this->estReset(buf_fill);
        return 0;
    }

    // 用来中断调用主线程
    {
        LockType lock(m_mutex);
        if (m_putNullPacket)
        {
            *quit = true;
            return NULL;
        }
    }

    // lock m_write & m_read
    {
        LockType lock(m_mutex);

        if (buf_fill >= BUFFER_FRAMES) 
        {   
            // overrunning! uh-oh. restart at a sane distance
            //std::cout << "overrun." << std::endl;
            m_read = m_write - m_start_fill;
        }

        read = m_read;
        m_read++;
    }

    buf_fill = m_write - m_read;
    this->estUpdate(buf_fill);

    volatile BufferType *curframe = m_buffer + BUFIDX(read);
    if (! curframe->ready) 
    {
        //std::cout << "missing frame." << std::endl;
        int frame_bytes = 4 * m_player->getFrameSize();
        memset(curframe->data, 0, frame_bytes);
    }

    curframe->ready = 0;
    return curframe->data;
}

int AudioBuffer::stuffBuffer(short *inptr, short *outptr) 
{
    int i;
    int stuffsamp = m_player->getFrameSize();
    int stuff = 0;
    double p_stuff;

    p_stuff = 1.0 - pow(1.0 - fabs(m_playback_rate - 1.0), m_player->getFrameSize());

    if (rand() < (p_stuff*RAND_MAX)) 
    {
        stuff = m_playback_rate > 1.0 ? -1 : 1;
        stuffsamp = rand() % (m_player->getFrameSize() - 1);
    }

    for (i=0; i<stuffsamp; i++) 
    {   
        // the whole frame, if no stuffing
        *outptr++ = dithered_vol(*inptr++);
        *outptr++ = dithered_vol(*inptr++);
    }

    if (stuff) 
    {
        if (stuff==1) 
        {
            // interpolate one sample
            *outptr++ = dithered_vol(((long)inptr[-2] + (long)inptr[0]) >> 1);
            *outptr++ = dithered_vol(((long)inptr[-1] + (long)inptr[1]) >> 1);
        }
        else if (stuff==-1) 
        {
            inptr++;
            inptr++;
        }
        
        for (i=stuffsamp; i<m_player->getFrameSize() + stuff; i++) 
        {
            *outptr++ = dithered_vol(*inptr++);
            *outptr++ = dithered_vol(*inptr++);
        }
    }

    return m_player->getFrameSize() + stuff;
}

void AudioBuffer::putPacket(SequenceType seqno, const char *data, int len) 
{
    //std::cout << "AudioBuffer::putPacket" << std::endl;
    
    volatile BufferType *abuf = NULL;
    short read = 0;
    short buf_fill = 0;
    
    m_putNullPacket = false;
    
    LockType lock(m_mutex);
    if (! m_synced) 
    {
        m_write = seqno;
        m_read = seqno - 1;
        m_synced = true;
    }

    if (seqno == m_write + 1) 
    {                  
        // expected packet
        abuf = m_buffer + BUFIDX(seqno);
        m_write = seqno;
    } 
    else if (seqOrder(m_write, seqno)) 
    {    
        // newer than expected
        this->rtp_request_resend(m_write, seqno - 1);

        abuf = m_buffer + BUFIDX(seqno);
        m_write = seqno;
    } 
    else if (seqOrder(m_read, seqno)) 
    {     
        // late but not yet played
        abuf = m_buffer + BUFIDX(seqno);
    } 
    else 
    {    // too late.
        printf("late packet %04X (%04X:%04X)\n", seqno, m_read, m_write);
    }

    buf_fill = m_write - m_read;

    if (abuf) 
    {
        m_player->alacDecode(abuf->data, data, len);
        abuf->ready = 1;
    }
    
    if (m_buffering && buf_fill >= m_start_fill)
    {
        //std::cout << "AudioBuffer::putPacket -> notify_one() " << std::endl;
        m_hasFrame.notify_one();
    }

    if (! m_buffering) 
    {
        // check if the t+10th packet has arrived... last-chance resend
        read = m_read + 10;
        abuf = m_buffer + BUFIDX(read);
        if (! abuf->ready)
            this->rtp_request_resend(read, read);
    }
}

void AudioBuffer::putNullPacket(void)
{
    LockType lock(m_mutex);
    m_putNullPacket = true;
    m_hasFrame.notify_one();
}

void AudioBuffer::estReset(short fill) 
{
    biquad_lpf(&m_drift_lpf, 1.0/180.0, 0.3);
    biquad_lpf(&m_err_lpf, 1.0/10.0, 0.25);
    biquad_lpf(&m_err_deriv_lpf, 1.0/2.0, 0.2);

    m_playback_rate = 1.0;
    m_est_err = 0;
    m_last_err = 0;
    m_desired_fill = 0;
    m_fill_count = 0;
}

void AudioBuffer::estUpdate(short fill) 
{
    if (m_fill_count < 1000) 
    {
        m_desired_fill += (double)fill/1000.0;
        m_fill_count++;
        return;
    }

    #define CONTROL_A   (1e-4)
    #define CONTROL_B   (1e-1)

    double buf_delta = fill - m_desired_fill;
    m_est_err = biquad_filt(&m_err_lpf, buf_delta);
    double err_deriv = biquad_filt(&m_err_deriv_lpf, m_est_err - m_last_err);

    m_est_drift = biquad_filt(&m_drift_lpf, CONTROL_B*(m_est_err*CONTROL_A + err_deriv) + m_est_drift);

    m_playback_rate = 1.0 + CONTROL_A * m_est_err + m_est_drift;

    m_last_err = m_est_err;
}

void AudioBuffer::biquad_init(biquad_t *bq, double a[], double b[]) 
{
    bq->hist[0] = bq->hist[1] = 0.0;
    memcpy(bq->a, a, 2*sizeof(double));
    memcpy(bq->b, b, 3*sizeof(double));
}

void AudioBuffer::biquad_lpf(biquad_t *bq, double freq, double Q) 
{
    double pi = M_PI;
    double w0 = 2*pi*freq/((float)m_player->getSamplingRate()/(float)m_player->getFrameSize());
    double alpha = sin(w0)/(2.0*Q);

    double a_0 = 1.0 + alpha;
    double b[3], a[2];
    b[0] = (1.0-cos(w0))/(2.0*a_0);
    b[1] = (1.0-cos(w0))/a_0;
    b[2] = b[0];
    a[0] = -2.0*cos(w0)/a_0;
    a[1] = (1-alpha)/a_0;

    biquad_init(bq, a, b);
}

double AudioBuffer::biquad_filt(biquad_t *bq, double in) 
{
    double w = in - bq->a[0]*bq->hist[0] - bq->a[1]*bq->hist[1];
	double out = bq->b[1]*bq->hist[0] + bq->b[2]*bq->hist[1] + bq->b[0]*w;
    bq->hist[1] = bq->hist[0];
    bq->hist[0] = w;

    return out;
}

void AudioBuffer::initBuffer(void)
{
    int outframe_bytes =  (4 * (m_player->getFrameSize() + 3) );

	if (! m_isBufferInit)
	{
		int i;
		for (i=0; i < BUFFER_FRAMES; i++)
			m_buffer[i].data = (short *) malloc(outframe_bytes);

		m_isBufferInit = true;
	}

	this->resync();
}

void AudioBuffer::destroyBuffer(void)
{
	if (m_isBufferInit)
	{
		int i;
		for (i=0; i < BUFFER_FRAMES; i++)
			free(m_buffer[i].data);

		m_isBufferInit = false;
	}
}

void AudioBuffer::rtp_request_resend(SequenceType first, SequenceType last) 
{
    if (seqOrder(last, first))
        return;

    unsigned char req[8];                                   // *not* a standard RTCP NACK
    req[0] = 0x80;
    req[1] = 0x55|0x80;                                     // Apple 'resend'
    *(unsigned short *)(req+2) = htons(1);                  // our seqnum
    *(unsigned short *)(req+4) = htons(first);              // missed seqnum
    *(unsigned short *)(req+6) = htons(last-first+1);       // count

    if (m_airport != NULL)
        m_airport->sendRtcpRequest(req, sizeof(req));
}

// the sequence numbers will wrap pretty often.
// this returns true if the second arg is after the first
int AudioBuffer::seqOrder(SequenceType a, SequenceType b) 
{
    signed short d = b - a;
    return d > 0;
}

short AudioBuffer::dithered_vol(short sample) 
{
    static short rand_a, rand_b;
    long out;

    out = (long)sample * m_player->fixVolume();
    if (m_player->fixVolume() < 0x10000) 
    {
		rand_b = rand_a;
		rand_a = lcg_rand();
        out += rand_a;
        out -= rand_b;
    }

    return out>>16;
}
