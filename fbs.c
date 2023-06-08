#include "headers.h"

extern int no_i;
extern BG fore, rProc[100];

void sig(char *str)
{
    pid_t pid;
    int job_num, sig_num;
    char *temp = strtok(str, " ");
    temp = strtok(NULL, " ");
    job_num = atoi(temp);
    temp = strtok(NULL, " ");
    sig_num = atoi(temp);

    if (job_num >= no_i || rProc[job_num].done == -1)
    {
        printf(RED);
        printf("error: no such job exists\n");
        printf(WHITE);
        return;
    }
    pid = rProc[job_num].id;
    kill(pid, sig_num);
}

void fg(char *str)
{
    pid_t pid;
    int job_num;
    char *temp = strtok(str, " ");
    temp = strtok(NULL, " ");
    job_num = atoi(temp);

    if (job_num >= no_i || rProc[job_num].done == -1)
    {
        printf(RED);
        printf("error: no such job exists\n");
        printf(WHITE);
        return;
    }
    pid = rProc[job_num].id;
    rProc[job_num].done = -1;

    fore.id = pid;
    strcpy(fore.pname, rProc[job_num].pname);

    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    kill(pid, SIGCONT);

    int status = 0;
    waitpid(pid, &status, WUNTRACED);

    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    fore.id = -1;
}

void bg(char *str)
{
    pid_t pid;
    int job_num;
    char *temp = strtok(str, " ");
    temp = strtok(NULL, " ");
    job_num = atoi(temp);

    printf("-%d %d-\n", job_num, no_i);
    if (job_num >= no_i || rProc[job_num].done == -1)
    {
        printf(RED);
        printf("error: no such job exists\n");
        printf(WHITE);
        return;
    }
    pid = rProc[job_num].id;
    kill(pid, SIGCONT);
}