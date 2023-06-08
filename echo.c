#include "headers.h"

void echo(char *str)
{
    char *temp = strtok(str, " ");
    temp = strtok(NULL, " ");
    if (temp == NULL)  // echo
    {
        printf("\n");
        return;
    }

    while (temp != NULL)
    {
        printf("%s", temp);
        temp = strtok(NULL, " ");
        if (temp != NULL)
            printf(" ");
    }
    printf("\n");
}