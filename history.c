#include "headers.h"

extern int ff, no_hist;
extern char hist[20][N];

void history(char *str)
{
    int k = no_hist - 10, c = 1;
    if (k < 0)
    {
        if (ff)
            k += 20;
        else
            k = 0;
    }

    while (1)
    {
        k %= 20;
        if (c > 10)
            break;
        if (!ff && k >= no_hist)
            break;
        printf("%d %s", c, hist[k]);
        k++;
        c++;
    }
}

void get_history()  // get the previous history
{
    FILE *fptr;
    fptr = fopen("history.txt", "r");
    if (fptr != NULL)
    {
        while (fgets(hist[no_hist], N, fptr))
        {
            no_hist++;
            no_hist %= 20;
            if (no_hist == 0)
                ff++;
        }
        fclose(fptr);
    }
}

void add_history()  // update the current history
{
    FILE *fptr = fopen("history.txt", "w");
    int k = 0, c = 1;
    if (ff)
        k = no_hist;
    while (1)
    {
        k %= 20;
        if (c > 20)
            break;
        if (!ff && k >= no_hist)
            break;
        fprintf(fptr, "%s", hist[k]);
        k++;
        c++;
    }
    fclose(fptr);
}