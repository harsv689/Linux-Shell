#include "headers.h"

BG fore;
BG rProc[100];   // store the background processes
List cProc[100]; // store the completed background processes

pid_t shell_pid;
int no_i = 1, no_c = 0;
int no_hist = 0, ff = 0, tt = 0;
char dir[N] = "~\0", home[N]; // home directory
char curr[N], prev[N] = "\0"; // current an previous directory
char hist[20][N];             // store the history of commands
char hostname[N], *username;

int fun(char *name, int b)
{
    int z = 0;
    int pin = 0, pout = 0;
    int in = 0, out = 0, scan = 0, arr[] = {0, 0, 0};
    char in_file[N] = "\0", out_file[N] = "\0", str[N] = "\0";

    // check for input-output redirection
    int j = 0, pos = 0, size = strlen(name);
    while (j < size)
    {
        if (name[j] == '<' || name[j] == '>')
            z = 0;

        if (z)
        {
            if (in && name[j] != ' ')
                in_file[pin++] = name[j];
            if (out && name[j] != ' ')
                out_file[pout++] = name[j];
        }

        if (name[j] == '<' || name[j] == '>')
        {
            z++;
            in = out = 0;
            if (name[j] == '<')
            {
                in++;
                arr[0]++;
            }
            else
            {
                out++;
                if (j + 1 < size && name[j + 1] == '>')
                {
                    j++;
                    arr[2]++;
                }
                else
                    arr[1]++;
            }
            scan++;
        }

        if (!scan)
            str[pos++] = name[j];
        j++;
    }
    str[pos] = '\0';
    in_file[pin] = '\0';
    out_file[pout] = '\0';

    z = 0;
    char com[N];
    size = strlen(str);
    for (int j = 0; j < size; j++)
    {
        if (str[j] == ' ' && z > 0)
            break;
        com[z++] = str[j];
    }
    com[z] = '\0';

    int sum = arr[0] + arr[1] + arr[2];
    if (arr[0] > 1 || arr[1] > 1 || arr[2] > 1 || sum > 2 || sum - arr[0] > 2)
    {
        printf(RED);
        printf("error: too many arguments\n");
        printf(WHITE);
        return 0;
    }

    int fd1, fd2, t_out = -1, t_in = -1;
    if (sum)
    {
        if (arr[0])
        {
            t_in = dup(STDIN_FILENO);
            fd1 = open(in_file, O_RDONLY);
            if (fd1 == -1)
            {
                printf(RED);
                printf("error: input file does not exists\n");
                printf(WHITE);
                return 0;
            }
            dup2(fd1, STDIN_FILENO);
        }

        if (arr[1] || arr[2])
        {
            t_out = dup(STDOUT_FILENO);
            if (arr[1])
                fd2 = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            else if (arr[2])
                fd2 = open(out_file, O_APPEND | O_CREAT | O_WRONLY, 0644);
            dup2(fd2, STDOUT_FILENO);
        }
    }

    if (b)
        bProcess(str, t_out); // call background process
    else if (strcmp(com, "pwd\0") == 0)
        pwd();
    else if (strcmp(com, "echo\0") == 0)
        echo(str);
    else if (strcmp(com, "cd\0") == 0)
        cd(str, t_out);
    else if (strcmp(com, "ls\0") == 0)
        ls(str, t_out);
    else if (strcmp(com, "pinfo\0") == 0)
        pinfo(str, t_out);
    else if (strcmp(com, "history\0") == 0)
        history(str);
    else if (strcmp(com, "discover\0") == 0)
        discover(str, t_out);
    else if (strcmp(com, "jobs\0") == 0)
        jobs(str);
    else if (strcmp(com, "sig\0") == 0)
        sig(str);
    else if (strcmp(com, "fg\0") == 0)
        fg(str);
    else if (strcmp(com, "bg\0") == 0)
        bg(str);
    else if (strcmp(com, "clear\0") == 0)
        printf("\e[1;1H\e[2J");
    else
        Process(str, t_out); // call foreground process

    if (arr[0])
        close(fd1);
    if (t_in != -1)
        dup2(t_in, STDIN_FILENO);
    if (arr[1] || arr[2])
        close(fd2);
    if (t_out != -1)
        dup2(t_out, STDOUT_FILENO);
    return 1;
}

void solve(char *name, int b)
{
    // check for pipes
    int x = 0;
    char num[100][N];
    char *temp = strtok(name, "|");
    while (temp != NULL)
    {
        if (temp[0] == ' ')
        {
            int idx = 0, pos = 0;
            int size = strlen(temp);
            char toll[N];
            for (int j = 0; j < size; j++)
            {
                if (temp[j] == ' ')
                {
                    if (!pos)
                        continue;
                }
                toll[idx++] = temp[j];
                pos++;
            }
            toll[idx] = '\0';
            strcpy(num[x++], toll);
        }
        else
            strcpy(num[x++], temp);
        temp = strtok(NULL, "|");
    }

    if (x == 1) // no pipes
        fun(name, b);
    else
    {
        int t_out, t_in, var;
        for (int j = 0; j < x; j++)
        {
            int size = strlen(num[j]);
            for (int h = size - 1; h >= 0; h--)
            {
                if (num[j][h] == ' ')
                    continue;
                num[j][h + 1] = '\0';
                break;
            }

            int fd[2];
            if (pipe(fd) < 0)
            {
                printf(RED);
                printf("error: could not create pipe\n");
                printf(WHITE);
                return;
            }
            t_in = dup(STDIN_FILENO);
            t_out = dup(STDOUT_FILENO);

            if (j == 0)
            {
                dup2(fd[1], STDOUT_FILENO);
                fun(num[j], 0);
            }
            else if (j == x - 1)
            {
                dup2(var, STDIN_FILENO);
                fun(num[j], b);
            }
            else
            {
                dup2(fd[1], STDOUT_FILENO);
                dup2(var, STDIN_FILENO);
                fun(num[j], 0);
            }
            var = fd[0];
            close(fd[1]);
            dup2(t_in, STDIN_FILENO);
            dup2(t_out, STDOUT_FILENO);
        }
    }
}

void call(int n, char input[][N])
{
    for (int i = 0; i < n; i++)
    {
        char name[N];
        int idx = 0, k = strlen(input[i]);
        for (int j = 0; j < k; j++)
        {
            if (input[i][j] == '\n')
                break;
            if (input[i][j] == '&')
            {
                name[idx] = '\0';
                solve(name, 1); // backgroung process
                idx = 0;
                continue;
            }

            if (input[i][j] == ' ' || input[i][j] == '\t')
            {
                if (idx > 0)
                {
                    if (name[idx - 1] != ' ')
                        name[idx++] = ' ';
                }
            }
            else
                name[idx++] = input[i][j];
        }
        name[idx] = '\0';
        solve(name, 0); // foreground process
    }
}

int main()
{
    get_history(); // get the previous history
    printf("\e[1;1H\e[2J");
    getcwd(home, N);          // get the current directory
    DIR *ptr = opendir(home); // open the home directory
    strcpy(curr, home);

    shell_pid = getpid();
    username = getlogin();                             // get username
    int ret = gethostname(hostname, sizeof(hostname)); // get hostname
    if (ret == -1)
    {
        printf(RED);
        printf("error: could not get hostname\n");
        printf(WHITE);
        return 0;
    }

    signal(SIGINT, ctrl_c);
    signal(SIGTSTP, ctrl_z);
    signal(SIGCHLD, handler);
    while (1)
    {
        prompt(username, hostname); // print prompt to terminal
        char *input = read_input();
        printf("\n");

        if (no_c) // print the completed background processes
        {
            for (int i = 0; i < no_c; i++)
            {
                printf("[%d] ", cProc[i].num);
                printf(PURPLE);
                printf("%s with pid ", cProc[i].pname);
                printf(BLUE);
                printf("%d", cProc[i].id);
                printf(PURPLE);
                if (cProc[i].status)
                    printf(" exited normally\n");
                else
                    printf(" exited abnormally\n");
                printf(WHITE);
            }
            no_c = 0;
        }

        int allow = 1, idx = no_hist - 1;
        if (!no_hist)
        {
            if (ff)
                idx = 19;
            else
                idx = -1;
        }

        if (idx != -1)
        {
            if (strcmp(hist[idx], input) == 0)
                allow = 0;
        }

        if (allow)
        {
            strcpy(hist[no_hist++], input); // update the history
            (no_hist) %= 20;
            if (!ff && no_hist == 0)
                ff++;
        }

        int z = 0;
        char num[100][N];
        char *temp = strtok(input, ";");
        while (temp != NULL)
        {
            strcpy(num[z++], temp);
            temp = strtok(NULL, ";");
        }
        call(z, num);
    }

    return 0;
}