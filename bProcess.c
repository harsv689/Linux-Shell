#include "headers.h"

extern int no_i;
extern BG rProc[100];

void bProcess(char *input, int t_out) // background processes
{
    if (t_out == -1)
        t_out = dup(STDOUT_FILENO);
    if (strlen(input) == 0)
        return;

    int z = 0;
    char *argv[10], toll[N], str[N];
    strcpy(toll, input);

    char *temp = strtok(input, " ");
    strcpy(str, temp);
    while (temp != NULL)
    {
        argv[z++] = temp;
        temp = strtok(NULL, " ");
    }
    argv[z] = NULL;

    int id = fork(); // create a new process
    if (id == 0)     // child
    {
        setpgrp();
        int ret = execvp(str, argv);
        if (ret == -1)
        {
            char buf[N];
            sprintf(buf, BLUE);
            write(t_out, buf, strlen(buf));
            sprintf(buf, "error: %s command not found\n", input);
            write(t_out, buf, strlen(buf));
            sprintf(buf, WHITE);
            write(t_out, buf, strlen(buf));
            exit(0);
        }
    }
    else // parent
    {
        char buf[N];
        sprintf(buf, "[%d] ", no_i);
        write(t_out, buf, strlen(buf));
        sprintf(buf, BLUE);
        write(t_out, buf, strlen(buf));
        sprintf(buf, "%d\n", id);
        write(t_out, buf, strlen(buf));
        sprintf(buf, WHITE);
        write(t_out, buf, strlen(buf));

        // store the background process
        rProc[no_i].id = id;
        rProc[no_i].num = no_i;
        rProc[no_i].done = 1;
        strcpy(rProc[no_i].pname, toll);
        no_i++;
        if (no_i == 100)
            no_i = 1;
    }
}