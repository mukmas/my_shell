#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char *string = NULL;
    size_t size = 0;
    ssize_t chars_read;

    chars_read = getline(&string, &size, stdin);

    free(string);

    return 0;
}
