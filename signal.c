#include "headers.h"

extern List cProc[100];
extern BG fore, rProc[100];
extern int no_i, no_c, shell_pid;
extern char hostname[N], *username;

void handler() // signal handler function
{
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    int k = WIFEXITED(status); // get the status of child process
    if (id > 0)
    {
        for (int i = 0; i < no_i; i++)
        {
            if (id == rProc[i].id)
            {
                cProc[no_c].id = id;
                cProc[no_c].num = rProc[i].num;
                cProc[no_c].status = k;
                strcpy(cProc[no_c].pname, rProc[i].pname);
                rProc[i].done = -1;
                no_c++;
                break;
            }
        }
    }
}

void ctrl_c()
{
    pid_t pid = getpid();
    if (fore.id > 0)
    {
        kill(fore.id, SIGINT);
        fore.id = -1;
        printf("\n");
    }
    else if (pid == shell_pid)
    {
        printf("\n");
        prompt(username, hostname);
        fflush(stdout);
    }
}

void ctrl_z()
{
    pid_t pid = getpid();
    if (fore.id > 0)
    {
        kill(fore.id, SIGTSTP);
        rProc[no_i].id = fore.id;
        rProc[no_i].num = no_i;
        rProc[no_i].done = 1;
        strcpy(rProc[no_i].pname, fore.pname);
        no_i++;
        if (no_i == 100)
            no_i = 1;
    }
    else if (pid == shell_pid)
    {
        printf("\n");
        prompt(username, hostname);
        fflush(stdout);
    }
}