#ifndef _HEADERS_H_
#define _HEADERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <termios.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#include "cd.h"
#include "ls.h"
#include "fbs.h"
#include "pwd.h"
#include "echo.h"
#include "jobs.h"
#include "input.h"
#include "pinfo.h"
#include "prompt.h"
#include "signal.h"
#include "process.h"
#include "history.h"
#include "bProcess.h"
#include "discover.h"

#define GREEN "\033[01;32m"
#define BLUE "\033[01;34m"
#define WHITE "\033[0;37m"
#define RED "\033[01;31m"
#define PURPLE "\033[01;35m"
#define YELLOW "\033[01;33m"
#define N 512

typedef struct BG
{
    int id;
    int num;
    int done;
    char pname[N];
} BG;

typedef struct list
{
    int id;
    int num;
    int status;
    char pname[N];
} List;

#endif