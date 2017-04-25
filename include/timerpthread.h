#ifndef PTHREADTIMER_H
#define PTHREADTIMER_H

#include <pthread.h>
#include <time.h>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>

#define SAVEDELAYTIME 5

/***********************************************************************************
*   Class screenSaver
***********************************************************************************/
class timerpthread_c
{
public:

    void (*ptrFunction)(void*) = NULL;

    timerpthread_c();
    ~timerpthread_c();
    void startThread();
    void stopThread();
    void cancelThread();
    void resetTimer();

protected:

private:
    pthread_t threadsTimer;
};


#endif // PTHREADTIMER_H
