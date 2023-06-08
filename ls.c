#include "headers.h"

extern char dir[N], home[N];
extern char prev[N], curr[N];

void ls_c(char *path, int a, int l, int is_file, char *file, int t_out)
{
    char buf[2 * N];
    struct dirent **arr;
    int n, flag = 0, t = 0, tt = 0;
    n = scandir(path, &arr, NULL, alphasort); // get all the files and folders
    if (!is_file)
    {
        for (int i = 0; i < n; i++)
        {
            struct stat it;
            char toll[N];
            strcpy(toll, path);
            strcat(toll, "/\0");
            strcat(toll, arr[i]->d_name);
            int k = stat(toll, &it);
            if (k == -1)
                continue;
            if (arr[i]->d_name[0] == '.')
                tt += it.st_blocks;
            else
                t += it.st_blocks;
        }
        if (l && a)
            printf("total %d\n", t + tt);
        else if (l)
            printf("total %d\n", t);
    }

    for (int i = 0; i < n; i++)
    {
        if (is_file)
        {
            if (strcmp(file, arr[i]->d_name) == 0)
                flag++;
            else
                continue;
        }

        if (arr[i]->d_name[0] == '.' && !a)
            continue;

        char val[20];
        struct stat it;
        char toll[N];
        strcpy(toll, path);
        strcat(toll, "/\0");
        strcat(toll, arr[i]->d_name);
        int k = stat(toll, &it);
        if (k == -1)
        {
            sprintf(buf, RED);
            write(t_out, buf, strlen(buf));
            sprintf(buf, "error: could not open : %s\n", toll);
            write(t_out, buf, strlen(buf));
            sprintf(buf, WHITE);
            write(t_out, buf, strlen(buf));
            return;
        }

        int is_dir = S_ISDIR(it.st_mode);
        sprintf(val, "%o", it.st_mode);
        int p = strlen(val), is_exec = 0;
        if ((val[p - 3] - '0') % 2 == 1)
            is_exec++;

        if (l)
        {
            if (is_dir)
                printf("d");
            else
                printf("-");

            for (int h = 0; h < 3; h++) // print permissions
            {
                int u = p + h - 3;
                int k = (val[u] - '0') % 4;
                if (val[u] - '0' > 3)
                    printf("r");
                else
                    printf("-");

                if (k == 2 || k == 3)
                    printf("w");
                else
                    printf("-");

                if ((val[u] - '0') % 2 == 1)
                    printf("x");
                else
                    printf("-");
            }
            printf(" %ld ", it.st_nlink); // number of hard links to the file

            struct passwd *pwd;
            pwd = getpwuid(it.st_uid);
            printf("%s ", pwd->pw_name); // user owner

            struct group *grp;
            grp = getgrgid(it.st_gid);
            printf("%s ", grp->gr_name);
            printf("%6ld ", it.st_size); // group owner

            int c = 0;
            char *data = ctime(&it.st_mtime);
            for (int i = 4;; i++)
            {
                if (data[i] == ':')
                    c++;
                if (c == 2)
                    break;

                printf("%c", data[i]); // last modified time
            }
            printf(" ");
        }

        if (is_dir)
            printf(BLUE);
        else if (is_exec)
            printf(GREEN);
        printf("%s\n", arr[i]->d_name);
        printf(WHITE);
        if (flag)
            break;
    }
    if (is_file && !flag)
    {
        sprintf(buf, RED);
        write(t_out, buf, strlen(buf));
        sprintf(buf, "cannot access '%s': No such file or directory\n", file);
        write(t_out, buf, strlen(buf));
        sprintf(buf, WHITE);
        write(t_out, buf, strlen(buf));
    }

    for (int i = 0; i < n; i++)
        free(arr[i]);
    free(arr);
}

void ls(char *str, int t_out)
{
    if (t_out == -1)
        t_out = dup(STDOUT_FILENO);

    char path[N];
    int a = 0, l = 0, z = 0;
    int no_dir = 0, no_file = 0, num[10];
    char *temp = strtok(str, " ");
    temp = strtok(NULL, " ");

    char arr[10][100], name[10][100];
    if (temp == NULL) // ls
    {
        strcpy(path, curr);
        ls_c(path, 0, 0, 0, "null\0", t_out);
        free(temp);
        return;
    }

    while (temp != NULL)
    {
        int k = strlen(temp);
        if (temp[0] == '-') // flag
        {
            if (temp[1] == 'a')
                a++;
            else if (temp[1] == 'l')
                l++;
            else
            {
                char buf[N];
                sprintf(buf, RED);
                write(t_out, buf, strlen(buf));
                sprintf(buf, "cannot access '%s': No such file or directory\n", temp);
                write(t_out, buf, strlen(buf));
                sprintf(buf, WHITE);
                write(t_out, buf, strlen(buf));
                return;
            }
            if (k > 1)
            {
                if (temp[2] == 'a')
                    a++;
                if (temp[2] == 'l')
                    l++;
            }
        }
        else // file or directory
        {
            if (temp[0] == '~') // ls ~
            {
                strcpy(path, home);
                int size = strlen(temp);

                char toll[N];
                if (size > 2) // ls ~/-->
                {
                    int idx = 0;
                    for (int i = 1; i < size; i++)
                        toll[idx++] = temp[i];
                    toll[idx] = '\0';
                    strcat(path, toll);
                }
            }
            else if (temp[0] == '.') // ls .
            {
                char toll[N];
                int size = strlen(temp);
                if (size > 1 && temp[1] == '.') // ls ..
                {
                    strcpy(toll, curr);
                    int size = strlen(curr);
                    for (int i = size - 1; i >= 0; i--)
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

                    if (size > 2) // ls ../-->
                    {
                        int idx = 0;
                        for (int i = 2; i < size; i++)
                            toll[idx++] = temp[i];
                        toll[idx] = '\0';
                        strcat(path, toll);
                    }
                }
                else // ls .
                {
                    strcpy(path, curr);
                    if (size > 1) // ls ./-->
                    {
                        int idx = 0;
                        for (int i = 1; i < size; i++)
                            toll[idx++] = temp[i];
                        toll[idx] = '\0';
                        strcat(path, toll);
                    }
                }
            }
            else if (temp[0] == '/') // ls /-->
                strcpy(path, temp);
            else // ls <dir_name>
            {
                strcpy(path, curr);
                strcat(path, "/\0");
                strcat(path, temp);
            }

            int r = chdir(path);
            if (r == -1) // file
            {
                num[z] = 1;
                no_file++;
            }
            else // directory
            {
                num[z] = 0;
                chdir(curr);
                no_dir++;
            }

            int size = strlen(path);
            for (int i = size - 1; i >= 0; i--)
            {
                if (path[i] == '/')
                {
                    int idx = 0;
                    for (int j = i + 1; j < size; j++)
                        name[z][idx++] = path[j];
                    name[z][idx] = '\0';
                    if (r == -1)
                    {
                        if (i == 0)
                        {
                            path[i] = '/';
                            path[i + 1] = '\0';
                        }
                        else
                            path[i + 1] = '\0';
                    }
                    break;
                }
            }
            strcpy(arr[z++], path);
        }
        temp = strtok(NULL, "\t ");
    }

    if (z == 0)
        ls_c(curr, a, l, 0, "null\0", t_out);
    else
    {
        int p = 0;
        if (no_dir + no_file > 1)
            p++;

        while (no_dir || no_file)
        {
            for (int i = 0; i < z; i++)
            {
                if (!no_dir && !no_file)
                    break;
                if (no_file)
                {
                    if (num[i])
                        no_file--;
                    else
                        continue;
                }
                else
                {
                    if (p)
                    {
                        printf(BLUE);
                        printf("%s :\n", name[i]);
                        printf(WHITE);
                    }
                    no_dir--;
                }
                chdir(arr[i]);
                ls_c(arr[i], a, l, num[i], name[i], t_out);
                chdir(curr);
            }
        }
    }
}