#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
void read_command(char* input) {
    printf("[Apathy-Shell] >> ");
    if (fgets(input, MAX_INPUT, stdin) == NULL) {
        perror("fgets");
        exit(1);
    }
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
}
void parse_command(char* input, char** args) {
    char* token = strtok(input, " ");
    int index = 0;

    while (token != NULL) {
        args[index++] = token;
        token = strtok(NULL, " ");
    }
    args[index] = NULL; 
}

int main() {
    char input[MAX_INPUT];
    char* args[64];

    while (1) {
        read_command(input);
        if (strcmp(input, "exit") == 0) {
            break;
        }
        parse_command(input, args);
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        } 
        else if (pid == 0) {
            if (execvp(args[0], args) == -1) {
                perror("execvp");
                exit(1);  
            }
        } 
        else {
            wait(NULL); 
        }
    }

    return 0;
}
