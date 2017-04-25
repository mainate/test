#ifndef PIPEOPEN_H
#define PIPEOPEN_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <string>
#include <sstream>

using namespace std;

#define READ   0
#define WRITE  1

FILE *popen2(const char *command, string type, int & pid);
int pclose2(FILE * fp, pid_t pid);
int pclose3(pid_t pid);

#endif // PIPEOPEN_H
