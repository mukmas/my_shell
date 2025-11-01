#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <pwd.h>

int print_prompt(void);
char *get_input(void);
void tokenize_input(int *count, char ***tokens, char *input);
int has_pipe(char **tokens, int count);
void normal_cmd(char **tokens, int count);
void free_memory(char **tokens, int count);

#define RESET "\x1b[0m"
#define RED "\x1b[31m"
#define YELLOW "\x1b[1;33m"
#define WHITE "\x1b[97m"

int main()
{

    while (1)
    {
        if (print_prompt() == 1)
        {
            continue;
        }

        char *input = get_input();
        if (input == NULL)
        {
            perror("Failed to read input");
            continue;
        }

        int count = 0;
        char **tokens = NULL;
        tokenize_input(&count, &tokens, input);
        free(input);

        if (count == 0 || tokens[0] == NULL)
        {
            continue;
        }

        // checks for builtin exit command
        if (strcmp(tokens[0], "exit") == 0)
        {
            free_memory(tokens, count);
            break;
        }

        // checks for builtin cd command
        if (strcmp(tokens[0], "cd") == 0)
        {
            char *dir = tokens[1];
            if (dir == NULL)
            {
                dir = getenv("HOME");
            }

            if (chdir(dir) == -1)
            {
                perror("cd failed");
            }
            free_memory(tokens, count);
            continue;
        }

        if (has_pipe(tokens, count))
        {
            // call function to run command with pipes
        }
        else
        {
            normal_cmd(tokens, count);
        }

        free_memory(tokens, count);
    }

    return 0;
}

// gets information about the shell user and prints it in a prompt
int print_prompt(void)
{
    // get username
    struct passwd *pw = getpwuid((getuid()));
    if (!pw)
    {
        perror("getuid failed");
        return 1;
    }

    char hostname[1024];
    if (gethostname(hostname, sizeof(hostname)) == -1)
    {
        perror("gethostname failed");
        return 1;
    }

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd failed");
        return 1;
    }

    // prints shell prompt
    printf(RED "%s@%s" WHITE ":" YELLOW "%s" RESET WHITE "$" RESET " ", pw->pw_name, hostname, cwd);
    return 0;
}

// reads a line of input from stdin
char *get_input(void)
{
    char *line = NULL;
    size_t size = 0;
    ssize_t chars_read;
    chars_read = getline(&line, &size, stdin);

    if (chars_read == -1)
    {
        free(line);
        return NULL;
    }

    return line;
}

// tokenizes input by making every word into a separate token
void tokenize_input(int *count, char ***tokens, char *input)
{
    char *token = strtok(input, " \t\n");

    char **temp = NULL;
    while (token != NULL)
    {
        temp = realloc(*tokens, sizeof(char *) * (*count + 2));
        if (temp == NULL)
        {
            free(*tokens);
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }
        *tokens = temp;
        temp[*count] = strdup(token);
        (*count)++;
        token = strtok(NULL, " \t\n");
    }
    if (*tokens != NULL)
        (*tokens)[(*count)] = NULL;
}

int has_pipe(char **tokens, int count)
{
    for (int i = 0; i < count; i++)
        if (strcmp(tokens[i], "|") == 0)
        {
            return 1;
        }
    return 0;
}

// executes a command in a forked process
void normal_cmd(char **tokens, int count)
{
    pid_t pid;

    pid = fork();
    switch (pid)
    {
    case -1:
        perror("fork");
        free_memory(tokens, count);
        exit(EXIT_FAILURE);

    case 0:
        execvp(tokens[0], tokens);
        exit(EXIT_FAILURE);

    default:
        wait(NULL);
        break;
    }
}

void free_memory(char **tokens, int count)
{
    for (int i = 0; i < count; i++)
    {
        free(tokens[i]);
    }
    free(tokens);
}