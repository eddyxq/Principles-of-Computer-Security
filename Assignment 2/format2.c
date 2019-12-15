#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s MESSAGE MAXLEN\n", argv[0]);
        return 1;
    }
    const char * message = argv[1];
    size_t maxlen = atol(argv[2]); // TODO: truncate string to this length...

    char forbidden_chars[12] = { '&', '$', '%', '/', '(', ')', ';', '|', '<', '>', '\\', '#' };

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

    return 0;
}
