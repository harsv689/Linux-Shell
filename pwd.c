#include "headers.h"

void pwd()
{
    char pwd[N];
    getcwd(pwd, N);
    printf("%s\n", pwd);
}