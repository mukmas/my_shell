#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

char *get_input(void);
void tokenize_input(int *count, char ***tokens, char *input);

int main()
{

    char *input = get_input();
    if (input == NULL)
    {
        perror("Failed to read input");
        return 1;
    }

    int count = 0;
    char **tokens = NULL;
    tokenize_input(&count, &tokens, input);
    free(input);

    pid_t pid;

    if (signal(SIGCHLD, SIG_IGN) == SIG_ERR)
    {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    switch (pid)
    {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);

    case 0:
        printf("I am child");
        exit(EXIT_SUCCESS);

    default:
        printf("I am parent");
    }

    for (int i = 0; i < count; i++)
    {
        free(tokens[i]);
    }
    free(tokens);

    printf("i returned 0");
    return 0;
}

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

void tokenize_input(int *count, char ***tokens, char *input)
{
    char *token = strtok(input, " \n");

    char **temp = NULL;
    while (token != NULL)
    {
        temp = realloc(*tokens, sizeof(char *) * (*count + 1));
        if (temp == NULL)
        {
            free(*tokens);
            free(input);
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }
        *tokens = temp;
        temp[*count] = strdup(token);
        (*count++);
        token = strtok(NULL, " \n");
    }
}