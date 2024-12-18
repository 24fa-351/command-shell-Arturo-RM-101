#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "function_paths.h"

void add_character_to_string (char *string, char character) {
    int length = strlen(string);
    string[length] = character;
    string[length + 1] = '\0';
}

void break_into_words (char* command, char* words[], char delimiter) {
    int total_words = 0;
    char *next_char = command;
    char current_word[10000];
    strcpy(current_word, "");

    while (*next_char != '\0') {

        if (*next_char == delimiter) {
            // Copy the new word into the child arguments
            words[total_words] = strdup(current_word);
            // The next place we'll put the argument
            total_words++;
            // Prepare for the next argument
            current_word[0] = '\0';
        } else {
            add_character_to_string(current_word, *next_char);
        }

        ++next_char;

    }

    words[total_words++] = strdup(current_word);

    words[total_words] = NULL;
}

int main(int argc, char *argv[]) {

    char line[1000];
    int output_fd = -1;
    int input_fd = -1;
    char absolute_path_to_command[1000];
    //char command = argv[2];

    //split(argv[2], words, ' ');
    // Move this into a function and call it through 51-54 named "execute shell"
    // 55-59 
    while(1) {

        printf("myshell> ");
        fgets(line, 1000, stdin);

        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        // End of execute command 


        if (strcmp(line, "exit") == 0 || strcmp(line, "quit") == 0) {
            break;
        }

        printf("You got: %s\n", line);
        char* words[1000];
        break_into_words(line, words, ' ');

        for(int ix = 0; words[ix] != NULL; ix++) {
            // single quotes help understand with each word
            printf("'%s'\n", words[ix]);
        }

        char* file_title;
        if (strcmp(words[2], ">") == 0) {
            file_title = words[3];
        } else {
            file_title = words[1];
        }

        execute_command(words, file_title);
        
        // This is where we'll be splitting our fgets() into cmds and args
        // Step 1: Get to break words (get it from redirect stream program, as well as finding absolute path from lecture)

        // Step 2: Look for anything of ">", "<", or ">>", OR "&" when use forkxee we don't use a wait
        // Step 3: Delete them if found and redirect them
        // Step 4: Execve the command
    }

}
