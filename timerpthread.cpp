#include "include/timerpthread.h"
#include"include/astrocurses.h"

static int saveDelay = {SAVEDELAYTIME};
static bool isThreadRunning = false;
//mutex
pthread_mutex_t mutexPthreadTimer;
static struct timespec currentclockthread, lastclockupdatethread;

extern textWindow* mainWin;

void* timerpthreadTimer(void *)
{
    // First run check if thread already running to start it
    //if (!mainWin->isRunningThread()) mainWin->startThread();
    do
    {
        clock_gettime(CLOCK_MONOTONIC, &currentclockthread);
        pthread_mutex_lock(&mutexPthreadTimer);
        if (((abs(currentclockthread.tv_sec - lastclockupdatethread.tv_sec) >= saveDelay)))
        {
            mainWin->stopThread();
            clock_gettime(CLOCK_MONOTONIC, &currentclockthread);
            lastclockupdatethread = currentclockthread;
            mainWin->startThread();
        }
        pthread_mutex_unlock(&mutexPthreadTimer);
        sleep(1);
    } while (isThreadRunning);
    pthread_exit(NULL);
};


/***********************************************************************************
*   Class screenSaver
***********************************************************************************/
timerpthread_c::timerpthread_c()
{
    isThreadRunning = false;
    //mutex
    if (pthread_mutex_init(&mutexPthreadTimer, NULL) != 0)
    {
        printf("\n mutex 'mutexPthreadTimer' init failed\n");
        exit(EXIT_FAILURE);
    }
}

timerpthread_c::~timerpthread_c()
{
    pthread_mutex_destroy(&mutexPthreadTimer);
}

void timerpthread_c::startThread()
{
    isThreadRunning = true;
    clock_gettime(CLOCK_MONOTONIC, &lastclockupdatethread);
    pthread_create(&threadsTimer, NULL, timerpthreadTimer, NULL);
}

void timerpthread_c::stopThread()
{
    isThreadRunning = false;
    pthread_join(threadsTimer, NULL);
}

void timerpthread_c::cancelThread()
{
    isThreadRunning = false;
    pthread_cancel(threadsTimer);
}

void timerpthread_c::resetTimer()
{
    pthread_mutex_lock(&mutexPthreadTimer);
    clock_gettime(CLOCK_MONOTONIC, &currentclockthread);
    lastclockupdatethread = currentclockthread;
    pthread_mutex_unlock(&mutexPthreadTimer);
}

