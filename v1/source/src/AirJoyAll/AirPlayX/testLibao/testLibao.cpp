/*
 *
 * testLibao.cpp
 *
 *
 */

#include <stdio.h>

//#include <windows.h>
#include <ao/ao.h>

int main(int argc, char** argv)
{
    FILE* fp = NULL;
    char buf[4096];
    int len = 0;
    ao_device* dev = NULL;
    ao_sample_format fmt = {0};

    if (argc < 2)
    {
        printf("Usage: %s file\n", argv[0]);
        printf("file -  wav file name\n");
        printf("example:\n");
        printf("%s demo.wav\n", argv[0]);
        return 0;
    }

    fp = fopen(argv[1], "rb");
    if (fp == NULL)
    {
        perror("fopen error/n");
        exit(-1);
    }

    ao_initialize();
   
#ifdef _WIN32 
    char *libao_driver	= "wmm";
#else
    char *libao_driver	= NULL;
#endif
    int driver;

    if (libao_driver) 
    {
        // if a libao driver is specified on the command line, use that
        driver = ao_driver_id(libao_driver);
        if (driver == -1) 
        {
            printf("Could not find requested ao driver\n");
        }
    }
    else 
    {
        // otherwise choose the default
        driver = ao_default_driver_id();
    }

#if 0
    fmt.bits = 16;
    fmt.rate = 8000;
    fmt.channels = 1;
    fmt.byte_format = AO_FMT_NATIVE;
#else
    fmt.bits = 16;
    //fmt.rate = 22000;
    fmt.rate = 44100;
    fmt.channels = 2;
    fmt.byte_format = AO_FMT_NATIVE;
#endif

    dev = ao_open_live(driver, &fmt, NULL);
    while (!feof(fp))
    {
        len = fread(buf, 1, 4096, fp);
        if (len > 0)
            ao_play(dev, buf, len);
    }

    ao_close(dev);
    ao_shutdown();
    
    fclose(fp);

    return 0;
}
