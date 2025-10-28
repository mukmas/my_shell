#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char *line = NULL;
    size_t size = 0;
    ssize_t chars_read;

    chars_read = getline(&line, &size, stdin);

    int count = 0;
    char **tokens = NULL;
    char *token = strtok(line, " \n");

    char **temp = NULL;
    while (token != NULL)
    {
        temp = realloc(tokens, sizeof(char *) * (count + 1));
        if (temp == NULL)
        {
            free(tokens);
            free(line);
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }
        tokens = temp;
        temp[count++] = strdup(token);
        token = strtok(NULL, " \n");
    }

    for (int i = 0; i < count; i++)
    {
        free(tokens[i]);
    }
    free(tokens);
    free(line);

    return 0;
}
