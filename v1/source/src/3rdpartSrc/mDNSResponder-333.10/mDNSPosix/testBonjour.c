#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "Bonjour.h"


// for mDNSCoreMachineSleep()
#include "mDNSEmbeddedAPI.h"
extern mDNS mDNSStorage;

void * thread_mdns(void *arg)
{
    printf("beging thread: mdns\n");
    mdnsd_start(0);
    printf("end thread: mdns\n");

    return NULL;
}


int main(int argc, char *argv[])
{
    pthread_t thread_id;
    int ret = 0;

    int i = 0;

    if (argc != 2)
    {
        printf("usage: %s second\n", argv[0]);
        return 0;
    }

    ret = pthread_create(&thread_id, NULL, thread_mdns, NULL);
    if (ret != 0)
    {
        perror("pthread_create");
        return 0;
    }

    printf("mdns daemon start ok!\n");

    i = atoi(argv[1]);
    int PowerResumeNotification = 0;
    while (i)
    {
        sleep(1);
        printf("wait %d\n", i);
        i --;

        PowerResumeNotification ++;

        if (PowerResumeNotification == 5)
        {
            mDNSCoreMachineSleep(&mDNSStorage, mDNSfalse);
        }
    }

    printf("----end----\n");

    return 0;
}
