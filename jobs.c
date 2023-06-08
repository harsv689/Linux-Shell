#include "headers.h"

extern int no_i;
extern BG rProc[100];

void jobs(char *str)
{
    int r = 0, s = 0;
    char *temp = strtok(str, " ");
    temp = strtok(NULL, " ");
    if (temp == NULL)
        r = s = 1;

    while (temp != NULL)
    {
        if (temp[0] == '-')
        {
            if (temp[1] == 'r')
                r++;
            if (temp[1] == 's')
                s++;
        }
        temp = strtok(NULL, " ");
    }

    BG names[no_i], var;
    for (int i = 0; i < no_i; i++)
        names[i] = rProc[i];

    int swap_count = 0, n = no_i;
    while (n > 1)
    {
        for (int i = 0; i < n - 1; i++)
        {
            int k = strcmp(names[i].pname, names[i + 1].pname);
            if (k > 0)
            {
                var = names[i];
                names[i] = names[i + 1];
                names[i + 1] = var;
                swap_count++;
            }
        }

        if (swap_count == 0)
            break;
        else
        {
            swap_count = 0;
            n--;
        }
    }

    for (int i = 0; i < no_i; i++)
    {
        if (names[i].done == -1)
            continue;

        char ch[10], a[N], b[N], name[N];
        sprintf(name, "/proc/%d/stat", names[i].id);
        FILE *fptr = fopen(name, "r");
        if (fptr == NULL)
            continue;

        fscanf(fptr, "%s %s %s", a, b, ch);
        if (s)
        {
            if (strcmp(ch, "T\0") == 0)
            {
                printf("[%d] ", names[i].num);
                printf("Stopped ");
                printf("%s [%d]\n", names[i].pname, names[i].id);
                continue;
            }
        }
        if (r)
        {
            if (strcmp(ch, "T\0") != 0)
            {
                printf("[%d] ", names[i].num);
                printf("Running ");
                printf("%s [%d]\n", names[i].pname, names[i].id);
            }
        }
        fclose(fptr);
    }
}