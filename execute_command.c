#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/wait.h>

//#include "function_paths.h"

int execute_command(char* words[], char file_title[]) {

    int output_fd = -1;
    int input_fd = -1;
    char absolute_path_to_command[1000];
    int word_count = 0;

    for (int ix = 0; words[ix] != NULL; ix++) {
        word_count++;
    }

    for (int ix = 0; ix < word_count; ix++) {
        if (words[ix] == NULL) {
            continue;
        }
        if (strcmp(words[ix], ">") == 0) {

            printf("Found >\n");
            int output_fd = open(words[ix + 1], O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR);

            if (output_fd == -1) {
                fprintf(stderr, "Failed to open %s\n", file_title);
                //fprintf(stderr, "Failed to open %s\n", argv[1]);
                return 1;
            }

            free(words[ix]);
            free(words[ix + 1]);

            // We don't worry about these anymore they're already taken care of
            words[ix] = NULL;
            words[ix + 1] = NULL;

        } else if (strcmp(words[ix], "<") == 0) {

            printf("Found <\n");
            int input_fd = open(words[ix - 1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
            if (input_fd == -1) {
                fprintf(stderr, "Failed to open %s\n", file_title);
                //fprintf(stderr, "Failed to open %s\n", argv[3]);
                return 1;
            }

            free(words[ix]);
            free(words[ix + 1]);

            words[ix] = NULL;
            words[ix + 1] = NULL;

        } else if (strcmp(words[ix], ">>") == 0) {

            printf("Found >>\n");

        } else {

            continue;

        }
    }

    int pid = fork();

    printf("The pid is: %d\n", pid);

    if (pid == 0) {
        if (output_fd > 0) {
            printf("Got here\n");
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }
        //printf("Got here\n");
        execve(absolute_path_to_command, words, NULL);
        printf("Unable to execute: %s", absolute_path_to_command);
        _exit(1);
    } else {
        if (output_fd > 0) {
            close(output_fd);
        }
        wait(NULL);
    }
}
