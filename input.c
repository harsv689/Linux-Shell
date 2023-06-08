#include "headers.h"

extern char curr[N], home[N];
extern char hostname[N], *username;
struct termios orig_termios;
void die(const char *s)
{
    perror(s);
    exit(1);
}

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

void predict(char *input, int *pt)
{
    int i, idx = -1, flag = 1, dir = 0;
    int size = strlen(input);
    char path[N], name[N];

    strcpy(path, curr);
    strcpy(name, input);
    for (int j = 0; j < size; j++)
    {
        if (input[j] == ' ')
            continue;
        if (input[j] == 'c' && input[j + 1] == 'd')
            dir++;
        else
            break;
    }

    for (i = size - 1; i >= 0; i--)
    {
        if (input[i] == ' ')
            break;
        if (input[i] == '/' && flag)
        {
            idx = i;
            flag--;
        }
    }

    if (idx != -1)
    {
        int z = 0;
        char temp[N];
        for (int j = i + 1; j < idx; j++)
            temp[z++] = input[j];
        temp[z] = '\0';

        z = 0;
        for (int j = idx + 1; j < size; j++)
            name[z++] = input[j];
        name[z] = '\0';

        if (temp[0] == '/')
            strcpy(path, temp);
        else
        {
            strcat(path, "/\0");
            strcat(path, temp);
        }
        chdir(path);
    }
    else
    {
        int z = 0;
        for (int j = i + 1; j < size; j++)
            name[z++] = input[j];
        name[z] = '\0';
    }

    int n, z = 0;
    char ext[N] = "\0";
    char files[100][N];
    struct dirent **arr;
    size = strlen(name);
    n = scandir(path, &arr, NULL, alphasort);
    for (int j = 0; j < n; j++)
    {
        if (arr[j]->d_name[0] == '.')
            continue;

        struct stat it;
        char toll[2 * N], temp[N];
        sprintf(toll, "%s/%s", path, arr[j]->d_name);
        strcat(toll, "\0");

        int k = stat(toll, &it);
        if (k == -1)
        {
            printf(RED);
            printf("error: could not open : %s\n", toll);
            printf(WHITE);
            return;
        }

        int is_dir = S_ISDIR(it.st_mode);
        int c = 0;
        for (int k = 0; k < size; k++)
        {
            if (arr[j]->d_name[k] != name[k])
                c++;
        }
        if (!c)
        {
            if (dir)
            {
                if (is_dir)
                {
                    strcpy(files[z++], arr[j]->d_name);
                    strcat(files[z - 1], "/\0");
                }
            }
            else
            {
                if (is_dir)
                {
                    strcpy(files[z++], arr[j]->d_name);
                    strcat(files[z - 1], "/\0");
                }
                else
                {
                    strcpy(files[z++], arr[j]->d_name);
                    strcat(files[z - 1], " \0");
                }
            }
        }
    }

    if (z)
        strcpy(ext, files[0]);
    for (int j = 0; j < z; j++)
    {
        size = strlen(files[j]);
        for (int k = 0; k < size; k++)
        {
            if (ext[k] != files[j][k])
            {
                ext[k] = '\0';
                break;
            }
        }
    }

    if (z > 1)
    {
        for (int j = 0; j < z; j++)
            printf("\n%s", files[j]);
    }
    printf("\n");

    idx = 0;
    i = strlen(name);
    size = strlen(ext);
    for (int j = i; j < size; j++)
    {
        (*pt)++;
        name[idx++] = ext[j];
    }
    name[idx] = '\0';
    strcat(input, name);

    prompt(username, hostname);
    printf("%s", input);

    chdir(curr);
    for (int i = 0; i < n; i++)
        free(arr[i]);
    free(arr);
}

char *read_input()
{
    char c;
    char *input = malloc(sizeof(char) * N);
    setbuf(stdout, NULL);
    enableRawMode();
    memset(input, '\0', N);

    int pt = 0;
    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (iscntrl(c))
        {
            if (c == 10)
            {
                input[pt++] = c;
                break;
            }
            else if (c == 127) // backspace
            {
                if (pt > 0)
                {
                    if (input[pt - 1] == 9)
                    {
                        for (int i = 0; i < 7; i++)
                            printf("\b");
                    }
                    input[--pt] = '\0';
                    printf("\b \b");
                }
            }
            else if (c == 9) // TAB character
                predict(input, &pt);
            else if (c == 4) // ctrl_d
            {
                printf("\n");
                chdir(home);
                add_history();
                exit(0);
            }
        }
        else
        {
            input[pt++] = c;
            printf("%c", c);
        }
    }
    disableRawMode();

    return input;
}