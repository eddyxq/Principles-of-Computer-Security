#include <sys/stat.h>  // sruct stat, stat
#include <unistd.h>    // sleep
#include <stdio.h>     // printf, snprintf, fflush, stdout
#include <string.h>    // strlen
#include <stdlib.h>    // system

int do_security_checks()
{
    printf("Performing security checks...");
    fflush(stdout);
    FILE * fd = popen("which echo", "r");
    char buf[64] = { '\0' };
    fread(buf, 1, sizeof(buf), fd);
    if (strcmp("/bin/echo\n", buf) != 0)
    {
        printf("Danger, Will Robinson! Aborting.\n");
        return 1;
    }
    //TODO: Implement more security checks; for now we pretend to
    // do some more useful stuff.
    sleep(2);
    printf("everything looks good! :)\n");
    return 0;
}

int main(int argc, char ** argv)
{
    if (argc < 2)
    {
        printf("Usage: %s message\n", argv[0]);
        return 1;
    }

    if (do_security_checks()) return 1;

    fflush(stdout);
    printf("ECHO: ");
    char * args[] = { "echo", argv[1], NULL };
    execvp(args[0], args);
}
