#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define LINE_LENGTH 1024
#define ARG_LENGTH 64

void parseArgs(char line[], int len, int *nargs_ptr, char **command_ptr, char ***arg_ptr) {
    for(int i=0; i<len; i++)
        if(line[i] == ' ') (*nargs_ptr)++;
    *arg_ptr = malloc((*nargs_ptr)*sizeof(char*));

    for(int i=0, i_arg=0, start=0; i<len; i++) {
        if(line[i] == ' ') {
            if(!i_arg) {
                *command_ptr = malloc((i+1)*sizeof(char));
                memcpy(*command_ptr, line, i);
                (*command_ptr)[i] = 0;
            } else {
                // TODO: this is where I think the error is
                (*arg_ptr)[i_arg] = malloc(50*sizeof(char));
                memcpy((*arg_ptr)[i_arg], line+start, i-start);
                (*arg_ptr)[i_arg][i-start+1] = 0;
            }

            start = i;
            i_arg++;
        }
    }
}

int main() {
    char line[LINE_LENGTH];
    int nargs, *nargs_ptr;
    nargs_ptr = &nargs;
    char **arg_ptr;
    char *command_ptr;

    // while(1) {
        memset(line, 0, LINE_LENGTH);
        scanf("%1023[^\n]", line);
        nargs = 0;
        parseArgs(line, LINE_LENGTH, nargs_ptr, &command_ptr, &arg_ptr);
        printf("%d\n", nargs);
        printf("%s\n", command_ptr);
        for(int i=0; i<nargs; i++)
            printf("%s\n", arg_ptr[i]);
    
        pid_t pid = fork();
        if(pid == -1) { // exit with error status
            exit(1);
        } else if(pid != 0) {
            // in parent process
            int status;
            waitpid(pid, &status, 0); // wait for child process to exit
        }
        else {
            // in child process
            execve(command_ptr, arg_ptr, 0);
        }
        free(arg_ptr);

    // }
}