#include "headers.h"

extern char dir[N], home[N];
extern char prev[N], curr[N];

void cd(char *str, int t_out)
{
    if (t_out == -1)
        t_out = dup(STDOUT_FILENO);

    char buf[N], path[N];
    char *temp = strtok(str, " ");
    temp = strtok(NULL, " ");
    if (temp == NULL) // cd
    {
        dir[0] = '~';
        dir[1] = '\0';
        chdir(home);
        strcpy(prev, curr);
        strcpy(curr, home);
        return;
    }

    char *arg = strtok(NULL, " ");
    if (arg != NULL)
    {
        sprintf(buf, RED);
        write(t_out, buf, strlen(buf));
        sprintf(buf, "error: cd: too many arguments\n");
        write(t_out, buf, strlen(buf));
        sprintf(buf, WHITE);
        write(t_out, buf, strlen(buf));
        return;
    }

    char toll[N];
    int size = strlen(temp);
    if (temp[0] == '~') // cd ~
    {
        strcpy(path, home);
        if (size > 2) // cd ~/-->
        {
            int idx = 0;
            for (int i = 1; i < size; i++)
                toll[idx++] = temp[i];
            toll[idx] = '\0';
            strcat(path, toll);
        }

        strcpy(prev, curr);
        strcpy(curr, path);
        strcpy(dir, path);
    }
    else if (temp[0] == '.') // cd .
    {
        if (size > 1 && temp[1] == '.') // cd ..
        {
            strcpy(toll, curr);
            int k = strlen(curr);
            for (int i = k - 1; i >= 0; i--)
            {
                if (toll[i] == '/')
                {
                    if (i == 0)
                    {
                        toll[i] = '/';
                        toll[i + 1] = '\0';
                    }
                    else
                        toll[i] = '\0';
                    break;
                }
            }
            strcpy(path, toll);

            if (size > 2) // cd ../-->
            {
                int idx = 0;
                for (int i = 2; i < size; i++)
                    toll[idx++] = temp[i];
                toll[idx] = '\0';
                strcat(path, toll);
            }
            else
            {
                if (strcmp(home, curr) == 0)
                    printf("%s\n", home);
            }
        }
        else
        {
            strcpy(path, curr);
            if (size > 1) // cd ./~
            {
                int idx = 0;
                for (int i = 1; i < size; i++)
                    toll[idx++] = temp[i];
                toll[idx] = '\0';
                strcat(path, toll);
            }
        }
        strcpy(prev, curr);
        strcpy(curr, path);
        strcpy(dir, path);
    }
    else if (strcmp(temp, "-\0") == 0) // cd -
    {
        if (prev[0] == '\0')
        {
            sprintf(buf, "cd: OLDPWD not set\n");
            write(t_out, buf, strlen(buf));
        }
        else
        {
            printf("%s\n", prev);
            strcpy(toll, prev);
            strcpy(prev, curr);
            strcpy(curr, toll);
            strcpy(dir, curr);
        }
    }
    else // cd <dir_name>
    {
        DIR *ptr = opendir(temp);
        if (ptr == NULL) // not a directory
        {
            // check for file
            sprintf(buf, RED);
            write(t_out, buf, strlen(buf));
            sprintf(buf, "error: cd: too many arguments\n");
            if (access(temp, F_OK) == 0)
                sprintf(buf, "error: %s: Not a directory\n", temp);
            else
                sprintf(buf, "error: %s: No such file or directory\n", temp);
            write(t_out, buf, strlen(buf));
            sprintf(buf, WHITE);
            write(t_out, buf, strlen(buf));
            closedir(ptr);
            return;
        }
        else
        {
            if (temp[0] == '/')
            {
                strcpy(path, temp);
                strcpy(prev, curr);
                strcpy(curr, path);
                strcpy(dir, path);
            }
            else
            {
                strcpy(prev, curr);
                int k = strlen(curr);
                if (curr[k - 1] != '/')
                    strcat(curr, "/\0");

                strcat(curr, temp);
                strcpy(dir, curr);
            }
        }
    }

    chdir(curr);
    int pp = strcmp(home, dir);
    if (pp == 0)
    {
        dir[0] = '~';
        dir[1] = '\0';
    }

    int x = strlen(home), y = strlen(dir);
    if (y > x)
    {
        int c = 0;
        for (int i = 0; i < x; i++)
        {
            if (home[i] == dir[i])
                c++;
            else
                break;
        }
        if (c == x)
        {
            int z = 0;
            dir[z++] = '~';
            for (int i = x; i < y; i++)
                dir[z++] = dir[i];
            dir[z] = '\0';
        }
    }
}