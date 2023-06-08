### Name : Amogha A Halhalli
### Roll No. : 2021101007

----
# USER-DEFINED INTERACTIVE SHELL PROGRAM

### Running the shell :
From the folder containing all the files, run
```
make
```
followed by
```
./main
```

### Supporting Files :
* main.c<br />
Read commands from the terminal and call appropriate functions.<br />
Considers the directory from which the shell is invoked as the home directory. 

* echo.c<br />
Prints user input after removing excess spaces/tabs.<br />
Does not handle quotes and environmnetal variables.<br />
If & or ; is encountered, then the input after & or ; is read as next command.

* cd.c<br />
Change to the directory specified by the user.<br />
Accepts only one argument after cd as first argument.

* pwd.c<br />
Prints the absolute path of the current working directory.

* ls.c<br />
Lists all the files and directories of the user specified directory.<br />
Supports multiple directories; flags handled are -a and -l.

* history.c<br />
Prints the latest 10 commands entered by the user.<br />
If there are fewer than 10 commands, prints all of them.

* input.c<br />
Uses rawmode to read input from stdin character by character.<br />
Supports autocompletion when 'tab' is entered.

* discover.c<br />
Search for file/directory in a user specified directory.<br />
Name of the file/directory to be found is given in quotes.<br />
Flags -d and -f lists all directories and files respectively.

* pinfo.c<br />
Prints the information of a process with given process ID.<br />
If process ID not given, considers the current running process.

* prompt.c<br />
Prints the appropriate prompt after the execution of every command.<br />
Prints the time taken for the foreground processes.

* process.c<br />
Executes all other background processes.<br />
Time taken is displayed by the prompt after its execution.

* bProcess.c<br />
Executes the command preceding '&' as background processes.<br />
Prints the process ID of the process at the start of its execution.<br />
Prints process ID and exited normally/abnormally after the execution.

* signal.c<br />
handler : Print the finished background processes along with their process ID.<br />
ctrl_c : Interrupt any currently running foreground job, by sending it the SIGINT signal.<br />
ctrl_z : Push any currently running foreground job into the background, and change its state from running to stopped.

* jobs.c<br />
Prints a list of all currently running background processes entered by the shell in
alphabetical order.<br />
Each line contains job number, process ID and their state(running or stopped).<br />
Flags handled are -r for running processes, -s for stopped processes and both(if not mentioned).

* fbs.c<br />
sig : Sends the corresponding signal to the process with given job number.<br />
fg : Brings the running or stopped background job of given job number to the foreground and changes its state to running.<br />
bg : Changes the state of a stopped background job to running in the background.

### Assumptions :
* The length of each input command should be less than 500.<br />
* The user is logged out of the terminal on entering ctrl_d.<br />
* The commands with &, | and ; are also considered as valid input commands.<br />
* The output of commands, usually written to stdout, can be redirected to another file.<br />
* The input taken from a file other than stdin, both input & output redirection can be used simultaneously.<br />
* Using < for read, > for write and >> for append, output file is created if it does not exist already.<br />
* It is assumed that there exists a command between two &'s, two |'s or two ;'s in the input.<br />
* The file headers.h contains all the header files and preprocessor directives.<br />
* Some global structs which are used are also defined in the file headers.h<br />
* The user commands are stored in a file history.txt for the command history.<br />
* The user is not allowed to change the contents of the file history.txt<br />
* The names of files and directories should not contain special characters.<br />
* Any error in the execution of commands will stop the execution and print error message.<br />
* Specific color coding is used for error messages, prompt, directories, executable files, etc.<br />