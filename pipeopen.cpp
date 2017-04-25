#include "include/pipeopen.h"

FILE *popen2(const char *command, string type, int & pid)
{
    pid_t child_pid;
    int fd[2];
    pipe(fd);

    if((child_pid = fork()) == -1)
    {
        perror("fork");
        exit(1);
    }

    /* child process */
    if (child_pid == 0)
    {
        if (type == "r")
        {
            close(fd[READ]);    //Close the READ end of the pipe since the child's fd is write-only
            dup2(fd[WRITE], 1); //Redirect stdout to pipe
        }
        else
        {
            close(fd[WRITE]);    //Close the WRITE end of the pipe since the child's fd is read-only
            dup2(fd[READ], 0);   //Redirect stdin to pipe
        }

        execl("/bin/sh", "/bin/sh", "-c", command, (char *)NULL);
//        execl("/bin/sh", "/bin/sh", "-c", command, NULL);
        exit(0);
    }
    else
    {
        if (type == "r")
        {
            close(fd[WRITE]); //Close the WRITE end of the pipe since parent's fd is read-only
        }
        else
        {
            close(fd[READ]); //Close the READ end of the pipe since parent's fd is write-only
        }
    }

    pid = child_pid;

    if (type == "r")
    {
        return fdopen(fd[READ], "r");
    }

    return fdopen(fd[WRITE], "w");
}

int pclose2(FILE * fp, pid_t pid)
{
    int stat;

    fclose(fp);
    while (waitpid(pid, &stat, 0) == -1)
    {
        if (errno != EINTR)
        {
            stat = -1;
            break;
        }
    }

    return stat;
}

int pclose3(pid_t pid)
{
    int internal_stat;
    waitpid(pid, &internal_stat, 0);
    return WEXITSTATUS(internal_stat);
}
