#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s MESSAGE\n", argv[0]);
        return 1;
    }
    const char * message = argv[1];

    char * forbidden_chars = (char *)malloc(12+1);
        forbidden_chars[0] = '&';
        forbidden_chars[1] = '$';
        forbidden_chars[2] = '/';
        forbidden_chars[3] = '(';
        forbidden_chars[4] = ')';
        forbidden_chars[5] = ';';
        forbidden_chars[6] = '|';
        forbidden_chars[7] = '<';
        forbidden_chars[8] = '>';
        forbidden_chars[9] = '\\';
        forbidden_chars[10] = '#';
        forbidden_chars[11] = '%';

    char buf[64];
    snprintf(buf, sizeof(buf), "Checking \"%.31s\" against blacklist...", message);
    printf(buf);

    const char *c = message;
    while (*c)
    {
        if (strchr(forbidden_chars, *c))
        {
            printf("\033[1m\033[31mFOUND '%c'!\033[0m\n", *c);
            return 1;
        }
        c++;
    }
    printf("\033[1m\033[33mDONE!\033[0m\n");

    snprintf(buf, sizeof(buf), "/bin/echo %s", message);
    printf("ECHO: "); fflush(stdout);
    system(buf);

    free(forbidden_chars);

    return 0;
}
