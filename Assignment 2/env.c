#include <stdio.h>  // printf, fflush
#include <unistd.h> // execvp

int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s message\n", argv[0]);
        return 1;
    }
    printf("ECHO: ");
    fflush(stdout);

    char * args[] = { "echo", argv[1], NULL };
    execvp("echo", args);

    return 0;
}
