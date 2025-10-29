#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

char *get_input(void);
void tokenize_input(int *count, char ***tokens, char *input);

int main()
{

    while (1)
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

        if (count == 0 || tokens[0] == NULL)
        {
            continue;
        }

        if (strcmp(tokens[0], "exit") == 0)
        {
            for (int i = 0; i < count; i++)
                free(tokens[i]);
            free(tokens);
            break;
        }

        pid_t pid;

        pid = fork();
        switch (pid)
        {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);

        case 0:
            execvp(tokens[0], tokens);
            exit(EXIT_FAILURE);

        default:
            wait(NULL);
            break;
        }

        for (int i = 0; i < count; i++)
        {
            free(tokens[i]);
        }
        free(tokens);
    }

    return 0;
}

char *get_input(void)
{
    char *line = NULL;
    size_t size = 0;
    ssize_t chars_read;

    printf("shell$ ");
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
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }
        *tokens = temp;
        temp[*count] = strdup(token);
        (*count)++;
        token = strtok(NULL, " \n");
    }
}