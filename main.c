#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char *string = NULL;
    size_t size = 0;
    ssize_t chars_read;

    chars_read = getline(&string, &size, stdin);

    int count = 0;
    char **tokens = NULL;
    char *token = strtok(string, " ");

    char **temp = NULL;
    while (token != NULL)
    {
        temp = realloc(tokens, sizeof(char *) * (count + 1));
        if (temp == NULL)
        {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }
        temp[count++] = token;
        token = strtok(NULL, " ");
    }
    tokens = temp;

    free(tokens);
    free(string);

    return 0;
}
