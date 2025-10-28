#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    for (int i = 0; i < count; i++)
    {
        free(tokens[i]);
    }
    free(tokens);

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
        temp[*count++] = strdup(token);
        token = strtok(NULL, " \n");
    }
    free(input);
}