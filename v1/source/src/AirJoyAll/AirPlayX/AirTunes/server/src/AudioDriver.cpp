/*
* AudioDriver.cpp
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

#include "AudioDriver.hpp"

#include <fstream>
#include <iostream>

using namespace airjoy;

#ifdef __USE_LIB_AO__

#ifdef _WIN32
    static char *libao_driver	= "wmm";
#else
    static char *libao_driver	= NULL;
#endif

static char *libao_devicename = NULL;
static char *libao_deviceid   = NULL; // ao_options expects "char*"

#define NUM_CHANNELS 2

//#define SAVE_TO_WAV_FILE

#ifdef SAVE_TO_WAV_FILE
static std::string gWavFilePath("f:/music/demo.wav");
std::ofstream gWavFile;
#endif // SAVE_TO_WAV_FILE

#ifdef _WIN32
AudioDriver::AudioDriver(AirTunesServerListener *listener)
    : m_dev(NULL)
    , m_handle(INVALID_HANDLE_VALUE)
    , m_samplingRate(0)
{
    //std::cout << "AudioDriver::AudioDriver()" << std::endl;

#ifdef SAVE_TO_WAV_FILE
    gWavFile.open(gWavFilePath.c_str(), std::ios_base::out | std::ios_base::binary);
#endif // SAVE_TO_WAV_FILE
}
#else
AudioDriver::AudioDriver(AirTunesServerListener *listener)
    : m_dev(NULL)
    , m_samplingRate(0)
{
}
#endif

AudioDriver::~AudioDriver()
{
    //std::cout << "AudioDriver::~AudioDriver()" << std::endl;

#if 0
    // 2013-2-26, 这里会导致崩溃，原因不明，暂时关闭
    if (m_dev != NULL)
    {
        ao_close(m_dev);
    }
#endif

#ifdef SAVE_TO_WAV_FILE
    gWavFile.close();
#endif // SAVE_TO_WAV_FILE
}

void AudioDriver::setSamplingRate(int samplingRate)
{
    m_samplingRate = samplingRate;
}

void AudioDriver::play(char *outbuf, int playSamples)
{
    //std::cout << "AudioDriver::play: " << std::endl;

    ao_play(m_dev, outbuf, playSamples);

#ifdef SAVE_TO_WAV_FILE
    for (std::size_t i = 0; i < (std::size_t)playSamples; ++i)
        gWavFile.put(outbuf[i]);
#endif // SAVE_TO_WAV_FILE
}

#ifdef _WIN32
void AudioDriver::setOutputFile(char *file)
{
    m_file = file; 
}

void AudioDriver::setOutputHandle(HANDLE handle)
{
    m_handle = handle;
}
#endif

void AudioDriver::init(void)
{
    //std::cout << "AudioDriver::init " << std::endl;

    this->init(libao_driver);
}

void AudioDriver::init(char *libao_driver) 
{
	if (m_dev == NULL)
	{
        // 2013-3-1
        // crash when runing on osx
        // move ao_initialize() for AirTunesServerHandler.cpp
		//ao_initialize();

		int driver;

    #ifdef _WIN32
		if (libao_driver != NULL)
		{
			// if a libao driver is specified on the command line, use that
			driver = ao_driver_id(m_file.empty() && m_handle == INVALID_HANDLE_VALUE ? libao_driver : m_handle != INVALID_HANDLE_VALUE ? "raw" : "au");
			if (driver == -1) 
            {
                std::cout << "Could not find requested ao driver" << std::endl;
			}
		}
		else 
        {
			// otherwise choose the default
			driver = ao_default_driver_id();
		}
    #else
        driver = ao_default_driver_id();
    #endif // _WIN32

		ao_sample_format fmt;
		memset(&fmt, 0, sizeof(fmt));

		fmt.bits = 16;
		fmt.rate = m_samplingRate;
		fmt.channels = NUM_CHANNELS;
		fmt.byte_format = AO_FMT_NATIVE;
	
		ao_option *ao_opts = NULL;
		if (libao_deviceid) 
        {
			ao_append_option(&ao_opts, "id", libao_deviceid);
		} 
        else if(libao_devicename)
        {
			ao_append_option(&ao_opts, "dev", libao_devicename);
		}

    #ifdef _WIN32
		if (m_file.empty() && m_handle == INVALID_HANDLE_VALUE)
		{
			m_dev = ao_open_live(driver, &fmt, ao_opts);
			if (m_dev == NULL) 
            {
                std::cout << "Could not open ao device" << std::endl;
			}
		}
		else
		{
			if (m_handle != INVALID_HANDLE_VALUE)
			{
				m_dev = ao_open_file2(driver, m_handle, &fmt, ao_opts);
				if (m_dev == NULL) 
                {
                    std::cout << "Could not open ao stdout device" << std::endl;
				}
			}
			else
			{
				m_dev = ao_open_file(driver, m_file.c_str(), TRUE, &fmt, ao_opts);
				if (m_dev == NULL) 
                {
                    std::cout << "Could not open ao file device" << std::endl;
				}
			}
		}
    #else

        m_dev = ao_open_live(driver, &fmt, ao_opts);
        if (m_dev == NULL) 
        {
            std::cout << "Could not open ao device" << std::endl;
        }
    #endif // _WIN32
	}
}

void AudioDriver::setFmtp(int fmtp[12], int len)
{
    for (int i=0; i < len; i++)
        m_fmtp[i] = fmtp[i];
}

#else // __USE_LIB_AO__

#include "AirTunesServerListener.hpp"

AudioDriver::AudioDriver(AirTunesServerListener *listener)
    : m_listener(listener)
    , m_samplingRate(0)
{
}

AudioDriver::~AudioDriver()
{
}

void AudioDriver::setSamplingRate(int samplingRate)
{
    m_samplingRate = samplingRate;
}

void AudioDriver::play(char *outbuf, int playSamples)
{
    //m_track.write(outbuf, playSamples);

    m_listener->didAudioDriverPlay(outbuf, playSamples);
}

void AudioDriver::init(void)
{
    // for android
    //m_track = new AudioTrack(AudioSystem::MUSIC, m_samplingRate, AudioSystem::PCM_16_BIT, AudioSystem::CHANNEL_OUT_MONO, 4*1024);

    m_listener->didAudioDriverInit(m_samplingRate, 2, 16, m_fmtp, 12);
}

void AudioDriver::setFmtp(int fmtp[12], int len)
{
    for (int i=0; i < len; i++)
        m_fmtp[i] = fmtp[i];
}
#endif // __USE_LIB_AO__
