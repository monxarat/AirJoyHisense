/*
* AudioPlayer.cpp
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

#include "AudioPlayer.hpp"
#include "AudioDriver.hpp"
#include "AudioBuffer.hpp"

using namespace airjoy;


#define MAX_PACKET      2048


AudioPlayer::AudioPlayer(AirTunesServerListener *listener)
    : m_listener(listener)
    , m_buffer(NULL)
    , m_isStart(false)
    , m_bufferPlaybackRate(1.0)
    , m_frameSize(0)
    , m_samplingRate(0)
    , m_volume(1.0)
    , m_fixVolume(0x10000)
    , m_decoder(NULL)
    , m_isInit(false)
{
    //std::cout << "AudioPlayer::AudioPlayer()" << std::endl;
}

AudioPlayer::~AudioPlayer()
{
    //std::cout << "AudioPlayer::~AudioPlayer()" << std::endl;

    this->destroyDecoder();
    this->stop();
}

void AudioPlayer::setBuffer(AudioBuffer *buffer)
{
    m_buffer = buffer;
}

void AudioPlayer::setFmtp(int fmtp[12], int len)
{
    for (int i=0; i < len; i++)
        m_fmtp[i] = fmtp[i];
}

void AudioPlayer::setAesIV(unsigned char aesiv[16], int len)
{
    for (int i=0; i < len; i++)
        m_aesiv[i] = aesiv[i];
}

void AudioPlayer::setAesKey(unsigned char aesKey[16], int len)
{
    for (int i=0; i < len; i++)
        m_aeskey[i] = aesKey[i];

    AES_set_decrypt_key(m_aeskey, 128, &m_aes);
}

bool AudioPlayer::start(void)
{
    if (! m_isInit)
        return false;

    if (m_isStart)
        return false;

    if (m_pThread.use_count() > 0)
        return false;

    try
    {
        m_pThread = boost::make_shared<boost::thread>(boost::bind(&AudioPlayer::thread, this));
    }
    catch (std::exception &e)
    {
        return false;
    }

    m_isStart = true;

    return true;
}

bool AudioPlayer::stop()
{
    if (! m_isStart)
        return false;

    if (m_pThread.use_count() == 0)
        return false;
    
    m_buffer->putNullPacket();

    m_pThread->join();
    m_pThread.reset();

    m_isStart = false;

    return true;
}

bool AudioPlayer::isStopped()
{
    return (! m_isStart);
}

int AudioPlayer::getFrameSize(void)
{
    return m_frameSize;
}

int AudioPlayer::getSamplingRate(void)
{
    return m_samplingRate;
}

long AudioPlayer::fixVolume(void)
{
    return m_fixVolume;
}

void AudioPlayer::flushDecoder(void)
{
    m_buffer->resync();
}

void AudioPlayer::volumeDecoder(double lfVolume)
{
	m_volume = pow(10.0, 0.05 * lfVolume);
	m_fixVolume = (long)(65536.0 * m_volume);
}

void AudioPlayer::thread(void)
{
    bool quit = false;
    int outframe_bytes =  (4 * (this->getFrameSize() + 3) );
    
	signed short *inbuf = NULL;
    signed short *outbuf = NULL;
    outbuf = (short *)malloc(outframe_bytes);

    while (true)
    {
        do
        {
            inbuf = m_buffer->getFrame(&quit);

            if (quit)
                break;
        }
        while (! inbuf);
            
        if (quit)
            break;

        int playSamples = m_buffer->stuffBuffer(inbuf, outbuf);

        m_driver->play((char *)outbuf, playSamples * 4);
    }

    free(outbuf);
}

bool AudioPlayer::initDecoder(void) 
{
    if (m_isInit)
        return false;

    m_isInit = true; 

	if (m_decoder != NULL)
		return true;

    m_frameSize = m_fmtp[1]; // stereo samples
    m_samplingRate = m_fmtp[11];

    int sample_size = m_fmtp[3];
    if (sample_size != 16)
    {
        std::cout << "sample_size : " << sample_size << std::endl;
        std::cout << "only 16-bit samples supported!" << std::endl;
        sample_size = 16;
    }

    alac_file *alac = create_alac(sample_size, 2);
    if (alac == NULL)
        return false;

    m_decoder = alac;

    alac->setinfo_max_samples_per_frame = m_frameSize;
    alac->setinfo_7a                    = m_fmtp[2];
    alac->setinfo_sample_size           = sample_size;
    alac->setinfo_rice_historymult      = m_fmtp[4];
    alac->setinfo_rice_initialhistory   = m_fmtp[5];
    alac->setinfo_rice_kmodifier        = m_fmtp[6];
    alac->setinfo_7f                    = m_fmtp[7];
    alac->setinfo_80                    = m_fmtp[8];
    alac->setinfo_82                    = m_fmtp[9];
    alac->setinfo_86                    = m_fmtp[10];
    alac->setinfo_8a_rate               = m_fmtp[11];
    allocate_buffers(alac);

    // 创建一个音频驱动
    m_driver = boost::make_shared<AudioDriver>(m_listener);
    m_driver->setSamplingRate(m_samplingRate);
    m_driver->setFmtp(m_fmtp, 12);
    m_driver->init();

    return true;
}

void AudioPlayer::destroyDecoder(void) 
{
	if (m_decoder != NULL)
	{
		destroy_alac(m_decoder);
		m_decoder = NULL;
	}
}

void AudioPlayer::alacDecode(short *dest, const char *buf, int len) 
{
    //std::cout << "AudioPlayer::alacDecode" << std::endl;

    unsigned char packet[MAX_PACKET];
    assert(len <= MAX_PACKET);

    unsigned char iv[16];
    int i = 0;
    memcpy(iv, m_aesiv, sizeof(iv));
    
    for (i=0; i+16<=len; i += 16)
        AES_cbc_encrypt((unsigned char*)buf+i, packet+i, 0x10, &m_aes, iv, AES_DECRYPT);
    
    if (len & 0xf)
        memcpy(packet+i, buf+i, len & 0xf);

    int outsize = 0;

    decode_frame(m_decoder, packet, dest, &outsize);

    int frame_bytes = 4 * m_frameSize;

    assert(outsize == frame_bytes);
}
