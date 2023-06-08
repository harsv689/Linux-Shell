#include "headers.h"

void pinfo(char *str, int t_out)
{
    if (t_out == -1)
        t_out = dup(STDOUT_FILENO);

    char pid[100] = "self\0";
    char *temp = strtok(str, " ");
    temp = strtok(NULL, " ");
    if (temp != NULL)
        strcpy(pid, temp);

    int z = 1, flag = 0;
    char arr[N], val[3][N];
    char name[N] = "/proc/\0";
    strcat(name, pid);
    strcat(name, "/status\0");
    FILE *fptr = fopen(name, "r");
    if (fptr == NULL)
    {
        char buf[N];
        sprintf(buf, RED);
        write(t_out, buf, strlen(buf));
        sprintf(buf, "error: not a valid pid\n");
        write(t_out, buf, strlen(buf));
        sprintf(buf, WHITE);
        write(t_out, buf, strlen(buf));
        return;
    }

    while (1)
    {
        fgets(arr, N, fptr);
        if (z == 3)
            strcpy(val[0], arr); // status
        if (z == 6)
            strcpy(val[1], arr); // pid
        if (z == 18)
            strcpy(val[2], arr); // memory
        z++;
        if (z > 18)
            break;
    }
    fclose(fptr);

    // pid
    int c = 0, idx = 0;
    int k = strlen(val[2]);
    for (int i = 0; i < k; i++)
    {
        if (val[1][i] == '\n')
            break;
        if (c)
        {
            if (val[1][i] == ' ' || val[1][i] == '\t')
                continue;
            pid[idx++] = val[1][i];
        }
        if (val[1][i] == ':')
            c++;
    }
    pid[idx] = '\0';
    printf("pid : %s\n", pid);

    // process status
    k = strlen(name);
    for (int i = k - 1; i >= 0; i--)
    {
        if (name[i] == 't')
        {
            name[i + 1] = '\0';
            break;
        }
    }

    char s, toll[N];
    char pgrp[N], tpgid[N];
    fptr = fopen(name, "r");
    fscanf(fptr, "%s %s %s %s %s %s %s %s", toll, toll, &s, toll, pgrp, toll, toll, tpgid);
    if (strcmp(pgrp, tpgid) == 0)
        flag++;
    fclose(fptr);

    c = idx = 0;
    k = strlen(val[0]);
    printf("process status : ");
    for (int i = 0; i < k; i++)
    {
        if (val[0][i] == '\n')
            break;
        if (c)
        {
            if (val[0][i] == ' ' || val[0][i] == '\t')
                continue;
            else
            {
                printf("%c", val[0][i]);
                break;
            }
        }
        if (val[0][i] == ':')
            c++;
    }
    if (flag)
        printf("+");
    printf("\n");

    // virtual memory
    c = idx = 0;
    char mem[100];
    k = strlen(val[2]);
    for (int i = 0; i < k; i++)
    {
        if (val[2][i] == '\n')
            break;
        if (c)
        {
            if (val[2][i] == ' ' || val[2][i] == '\t')
                continue;
            mem[idx++] = val[2][i];
        }
        if (val[2][i] == ':')
            c++;
    }
    mem[idx] = '\0';
    printf("memory : %s\n", mem);

    // executable path
    char buf[1024], path[N] = "/proc/\0";
    strcat(path, pid);
    strcat(path, "/exe\0");
    ssize_t len = readlink(path, buf, 1023);
    buf[len] = '\0';
    printf("executable Path : %s\n", buf);
}