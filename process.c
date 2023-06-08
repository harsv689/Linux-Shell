#include "headers.h"

extern int tt;
extern BG fore;
extern List cProc[100];

void Process(char *input, int t_out) // foreground processes
{
    if (t_out == -1)
        t_out = dup(STDOUT_FILENO);
    if (strlen(input) == 0)
        return;

    int z = 0;
    char *argv[10], str[N], toll[N];
    strcpy(toll, input);

    time_t begin, end; // compute time
    char *temp = strtok(input, " ");
    strcpy(str, temp);
    while (temp != NULL)
    {
        argv[z++] = temp;
        temp = strtok(NULL, " ");
    }
    argv[z] = NULL;

    pid_t id = fork(); // create a new process
    if (id == -1)
    {
        char buf[N];
        sprintf(buf, BLUE);
        write(t_out, buf, strlen(buf));
        sprintf(buf, "could not create new process\n");
        write(t_out, buf, strlen(buf));
        sprintf(buf, WHITE);
        write(t_out, buf, strlen(buf));
        return;
    }

    begin = time(NULL);
    if (id == 0) // child
    {
        int ret = execvp(str, argv);
        if (ret == -1)
        {
            char buf[2 * N];
            sprintf(buf, RED);
            write(t_out, buf, strlen(buf));
            sprintf(buf, "error: %s command not found\n", toll);
            write(t_out, buf, strlen(buf));
            sprintf(buf, WHITE);
            write(t_out, buf, strlen(buf));
            exit(0);
        }
    }
    else // parent
    {
        strcpy(fore.pname, toll);
        fore.id = id;
        
        int status;
        waitpid(id, &status, WUNTRACED | WCONTINUED);
        end = time(NULL);
        if (end > begin)
            tt += end - begin;
        fore.id = -1;
    }
}