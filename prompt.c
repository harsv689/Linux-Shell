#include "headers.h"

extern int tt;
extern char dir[N];

void prompt(char *username, char *hostname)
{
    printf("<");
    printf(GREEN);
    printf("%s@%s:", username, hostname);
    printf(BLUE);
    printf("%s", dir);
    if (tt != 0)
    {
        printf(PURPLE);
        printf(" took %dsec ", tt);
        tt = 0;
    }
    printf(WHITE);
    printf("> ");
}