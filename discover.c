#include "headers.h"

extern char curr[N], home[N];

void discover_d(char *path, char *drt, int *arr, int t_out, char *name)
{
    struct dirent *dp;
    DIR *pt = opendir(path);
    if (pt == NULL)
    {
        char buf[N];
        sprintf(buf, RED);
        write(t_out, buf, strlen(buf));
        sprintf(buf, "error: could not open directory %s\n", path);
        write(t_out, buf, strlen(buf));
        sprintf(buf, WHITE);
        write(t_out, buf, strlen(buf));
        return;
    }

    int z = 0;
    char val[100][N];
    while ((dp = readdir(pt)) != NULL) // read all the files and folders
    {
        if (dp->d_name[0] == '.')
            continue;

        struct stat it;
        char toll[N];
        strcpy(toll, path);
        strcat(toll, "/\0");
        strcat(toll, dp->d_name);
        int k = stat(toll, &it);
        if (k == -1)
            continue;

        int is_dir = S_ISDIR(it.st_mode);
        if (is_dir)
        {
            if (arr[3]) // if search for a directory
            {
                if (strcmp(name, dp->d_name) == 0)
                {
                    arr[3] = 0;
                    printf("%s/%s\n", drt, dp->d_name);
                    closedir(pt);
                    return;
                }
            }

            if (arr[0]) // if d flag is set
                printf("%s/%s\n", drt, dp->d_name);

            char file[N];
            strcpy(file, drt);
            k = strlen(file);
            if (file[k - 1] != '/')
                strcat(file, "/\0");
            strcat(file, dp->d_name);
            discover_d(toll, file, arr, t_out, name); // recursively search in directories
        }
        else
        {
            if (arr[2]) // if search for a file
            {
                if (strcmp(name, dp->d_name) == 0)
                {
                    arr[2] = 0;
                    printf("%s/%s\n", drt, dp->d_name);
                    closedir(pt);
                    return;
                }
            }

            if (arr[1]) // if f flag is set
                printf("%s/%s\n", drt, dp->d_name);
        }
    }
    closedir(pt);
}

void discover(char *str, int t_out)
{
    if (t_out == -1)
        t_out = dup(STDOUT_FILENO);

    chdir(curr);
    char path[N], tdir[N], drt[N], name[N];
    int z = 0, flag = 0, no_file = 0, no_dir = 0;

    int arr[] = {0, 0, 0, 0};
    char *temp = strtok(str, " ");
    temp = strtok(NULL, " ");
    strcpy(path, curr);
    strcpy(drt, ".\0");
    if (temp == NULL) // discover
    {
        arr[0] = arr[1] = 1;
        discover_d(path, drt, arr, t_out, "null\0");
        return;
    }

    strcpy(tdir, curr);
    while (temp != NULL)
    {
        int k = strlen(temp);
        if (temp[0] == '-') // flag
        {
            if (temp[1] == 'd')
                arr[0]++;
            else if (temp[1] == 'f')
                arr[1]++;
            else
            {
                char buf[N];
                sprintf(buf, RED);
                write(t_out, buf, strlen(buf));
                sprintf(buf, "error: cannot access '%s': No such file or directory\n", temp);
                write(t_out, buf, strlen(buf));
                sprintf(buf, WHITE);
                write(t_out, buf, strlen(buf));
                return;
            }
            flag++;
        }
        else // file or directory
        {
            if (temp[0] == '"') // if there is search
            {
                int idx = 0, k = strlen(temp);
                for (int i = 1; i < k; i++)
                {
                    if (temp[i] == '"')
                        break;
                    temp[idx++] = temp[i];
                }
                temp[idx] = '\0';
                strcpy(name, temp);
                z++;
                arr[2]++;
                arr[3]++;
            }

            if (!z) // if there is no search
            {
                if (temp[0] == '~') // home directory
                {
                    strcpy(path, home);
                    int size = strlen(temp);

                    char toll[N];
                    if (size > 2)
                    {
                        int idx = 0;
                        for (int i = 1; i < size; i++)
                            toll[idx++] = temp[i];
                        toll[idx] = '\0';
                        strcat(path, toll);
                    }
                }
                else if (temp[0] == '.') // current or previous directory
                {
                    char toll[N];
                    int size = strlen(temp);
                    if (size > 1 && temp[1] == '.') // previous directory
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

                        if (size > 2)
                        {
                            int idx = 0;
                            for (int i = 2; i < size; i++)
                                toll[idx++] = temp[i];
                            toll[idx] = '\0';
                            strcat(path, toll);
                        }
                    }
                    else // current directory
                    {
                        strcpy(path, curr);
                        if (size > 1)
                        {
                            int idx = 0;
                            for (int i = 1; i < size; i++)
                                toll[idx++] = temp[i];
                            toll[idx] = '\0';
                            strcat(path, toll);
                        }
                    }
                }
                else if (temp[0] == '/')
                    strcpy(path, temp);
                else // mentioned directory
                {
                    strcpy(path, curr);
                    strcat(path, "/\0");
                    strcat(path, temp);
                }
            }
            else
                strcpy(path, temp);

            int r = chdir(path);
            if (r == -1) // file
            {
                no_file++;
                strcpy(name, temp);
            }
            else // directory
            {
                no_dir++;
                if (!z)
                {
                    strcpy(tdir, path);
                    strcpy(drt, temp);
                }
                int l = strlen(drt);
                if (drt[l - 1] == '/')
                    drt[l - 1] = '\0';
            }

            int i, k = strlen(temp);
            for (i = k - 1; i >= 0; i--)
            {
                if (temp[i] == '/')
                    break;
            }

            if (i == 0)
                strcpy(name, temp);
            else
            {
                int idx = 0;
                for (int j = i + 1; j < k; j++)
                    name[idx++] = temp[j];
                name[idx] = '\0';
            }
        }
        z = 0;
        temp = strtok(NULL, " ");
    }

    if (arr[3] || arr[2])
    {
        if (flag)
        {
            arr[3] = arr[0];
            arr[2] = arr[1];
        }
        discover_d(tdir, drt, arr, t_out, name);
    }
    else
    {
        printf("%s\n", drt);
        discover_d(tdir, drt, arr, t_out, name);
    }
}